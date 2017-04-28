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

#include "RadarApp.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "AppCli_m.h"
#include "inet/linklayer/common/Ieee802Ctrl.h"
#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/common/ModuleAccess.h"



Define_Module(RadarApp);

simsignal_t RadarApp::sentPkSignal = registerSignal("sentPk");
simsignal_t RadarApp::rcvdPkSignal = registerSignal("rcvdPk");

simsignal_t RadarApp::rogueSignal = registerSignal("rogue");
simsignal_t RadarApp::mineSignal = registerSignal("mine");

simsignal_t RadarApp::rogueDetectSignal = registerSignal("rogueDetection");
simsignal_t RadarApp::mineDetectSignal = registerSignal("mineDetection");


RadarApp::~RadarApp()
{
    cancelAndDelete(timerMsg);
}

void RadarApp::initialize(int stage)
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

        keys = par("keys").stringValue();

        size_t pos = 0;
        std::string token;
        std::string delimiter = ";";
        while ((pos = keys.find(delimiter)) != std::string::npos) {
            token = keys.substr(0, pos);
            keyChain.insert(token);
            keys.erase(0, pos + delimiter.length());
        }

        if (isGenerator()){
            timerMsg = new cMessage("generateNextPacket");
            scheduleNextPacket(true);
        }
    }
}

void RadarApp::handleMessage(cMessage *msg)
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

bool RadarApp::handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback)
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

bool RadarApp::isGenerator()
{
    return par("destAddress").stringValue()[0];
}

void RadarApp::scheduleNextPacket(bool start)
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

void RadarApp::cancelNextPacket()
{
    if (timerMsg)
        cancelEvent(timerMsg);
}

MACAddress RadarApp::resolveDestMACAddress()
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

void RadarApp::sendPacket()
{
    seqNum++;

    char msgname[30];
    sprintf(msgname, "req-%d-%ld", getId(), seqNum);
    EV_INFO << "Generating packet `" << msgname << "'\n";

    EtherAppPct *datapacket = new EtherAppPct(msgname, IEEE802CTRL_DATA);
    datapacket->setMacAddress( myMAC.c_str() );
    datapacket->setNodeId( getParentModule()->getIndex() );

    Ieee802Ctrl *etherctrl = new Ieee802Ctrl();
    etherctrl->setDest(destMACAddress);
    datapacket->setControlInfo(etherctrl);

    emit(sentPkSignal, datapacket);
    send(datapacket, "out");
    packetsSent++;
}

void RadarApp::receivePacket(cPacket *msg)
{
    int value = 0;
    EtherAppPct *pct = static_cast< EtherAppPct *>(msg);

    std::string key = pct->getKey();
    int nodeRcvd = pct->getNodeId();
    std::string nodemac = pct->getMacAddress();

    // Get drone node to display a bubble
    cModule *mod = simulation.getModule( nodeRcvd );

    std::set<std::string>::iterator it = keyChain.find(key);
    if( it == keyChain.end() )
    {
        std::map<std::string, int>::iterator it = rogue_drones.find(nodemac);
        if(it == rogue_drones.end())
        {
            rogue_drones.insert( std::pair<std::string, int>(nodemac, 1) );
            emit(rogueDetectSignal, simTime() );
        }
        else
        {
            value = rogue_drones.find(nodemac)->second;
            it->second = (value+1);
        }

        if(hasGUI())
            mod->bubble("ROGUE");

        emit(rogueSignal, msg);
    }
    else
    {
        std::map<std::string, int>::iterator it = my_drones.find(key);
        if(it == my_drones.end())
        {
            my_drones.insert( std::pair<std::string, int>(key, 1) );
            emit(mineDetectSignal, simTime() );
        }
        else
        {
            value = my_drones.find(key)->second;
            it->second = (value+1);
        }

        if(hasGUI())
            mod->bubble("MINE");

        emit(mineSignal, msg);
    }

    packetsReceived++;
    emit(rcvdPkSignal, msg);
    delete msg;
}

void RadarApp::finish()
{
    cancelAndDelete(timerMsg);
    timerMsg = nullptr;
}

