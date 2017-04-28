//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "DroneApp.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "AppCli_m.h"
#include "inet/linklayer/common/Ieee802Ctrl.h"
#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/common/ModuleAccess.h"



Define_Module(DroneApp);

simsignal_t DroneApp::sentPkSignal = registerSignal("sentPk");
simsignal_t DroneApp::rcvdPkSignal = registerSignal("rcvdPk");

DroneApp::~DroneApp()
{
    cancelAndDelete(timerMsg);
}

void DroneApp::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        reqLength = &par("reqLength");
        respLength = &par("respLength");
        sendInterval = &par("sendInterval");

        seqNum = 0;
        WATCH(seqNum);

        // statistics
        packetsSent = packetsReceived = 0;
        WATCH(packetsSent);
        WATCH(packetsReceived);

        startTime = par("startTime");
        stopTime = par("stopTime");

        if (stopTime >= SIMTIME_ZERO && stopTime < startTime)
            throw cRuntimeError("Invalid startTime/stopTime parameters");
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {

        cModule *mod = getParentModule()->getSubmodule("wlan")->getSubmodule("mac");
        myMAC = mod->par("address").str();

        key = &par("key");


        if (isGenerator()){
            timerMsg = new cMessage("generateNextPacket");
            scheduleNextPacket(true);
        }
    }
}

void DroneApp::handleMessage(cMessage *msg)
{

    if (msg->isSelfMessage()) {
        if (msg->getKind() == START) {
            destMACAddress = resolveDestMACAddress();

            // if no dest address given, nothing to do
            if (destMACAddress.isUnspecified())
                return;
        }
        sendPacket();
        scheduleNextPacket(false);
    }
    else
        receivePacket(check_and_cast<cPacket *>(msg));
}

bool DroneApp::handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback)
{
    Enter_Method_Silent();
    if (dynamic_cast<NodeStartOperation *>(operation)) {
        if ((NodeStartOperation::Stage)stage == NodeStartOperation::STAGE_APPLICATION_LAYER && isGenerator())
            scheduleNextPacket(true);
    }
    else if (dynamic_cast<NodeShutdownOperation *>(operation)) {
        if ((NodeShutdownOperation::Stage)stage == NodeShutdownOperation::STAGE_APPLICATION_LAYER)
            cancelNextPacket();
    }
    else if (dynamic_cast<NodeCrashOperation *>(operation)) {
        if ((NodeCrashOperation::Stage)stage == NodeCrashOperation::STAGE_CRASH)
            cancelNextPacket();
    }
    else
        throw cRuntimeError("Unsupported lifecycle operation '%s'", operation->getClassName());
    return true;
}

bool DroneApp::isGenerator()
{
    return par("destAddress").stringValue()[0];
}

void DroneApp::scheduleNextPacket(bool start)
{
    simtime_t cur = simTime();
    simtime_t next;
    if (start) {
        next = cur <= startTime ? startTime : cur;
        timerMsg->setKind(START);
    }
    else {
        next = cur + sendInterval->doubleValue();
        timerMsg->setKind(NEXT);
    }
    if (stopTime < SIMTIME_ZERO || next < stopTime)
        scheduleAt(next, timerMsg);
}

void DroneApp::cancelNextPacket()
{
    if (timerMsg)
        cancelEvent(timerMsg);
}

MACAddress DroneApp::resolveDestMACAddress()
{
    MACAddress destMACAddress;
    const char *destAddress = par("destAddress");
    if (destAddress[0]) {
        // try as mac address first, then as a module
        if (!destMACAddress.tryParse(destAddress)) {
            cModule *destStation = getModuleByPath(destAddress);
            if (!destStation)
                throw cRuntimeError("cannot resolve MAC address '%s': not a 12-hex-digit MAC address or a valid module path name", destAddress);

            cModule *destMAC = destStation->getSubmodule("mac");
            if (!destMAC)
                throw cRuntimeError("module '%s' has no 'mac' submodule", destAddress);

            destMACAddress.setAddress(destMAC->par("address"));
        }
    }
    return destMACAddress;
}

void DroneApp::sendPacket()
{
    seqNum++;

    char msgname[30];
    sprintf(msgname, "req-%d-%ld", getId(), seqNum);
    EV_INFO << "Generating packet `" << msgname << "'\n";

    EtherAppPct *datapacket = new EtherAppPct(msgname, IEEE802CTRL_DATA);
    datapacket->setMacAddress( myMAC.c_str() );
    datapacket->setNodeId( getParentModule()->getId() );
    datapacket->setKey( key->stringValue() );

    Ieee802Ctrl *etherctrl = new Ieee802Ctrl();
    etherctrl->setDest(destMACAddress);
    datapacket->setControlInfo(etherctrl);

    emit(sentPkSignal, datapacket);
    send(datapacket, "out");
    packetsSent++;
}

void DroneApp::receivePacket(cPacket *msg)
{
    EV << "Drone[" << getParentModule()->getIndex() << "] RECEBEU UM PACOTE\n";

    packetsReceived++;
    emit(rcvdPkSignal, msg);
    delete msg;
}

void DroneApp::finish()
{
    cancelAndDelete(timerMsg);
    timerMsg = nullptr;
}

