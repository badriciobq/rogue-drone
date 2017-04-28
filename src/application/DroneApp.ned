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

package vant_wifi.application;

//
// TODO auto-generated module
//
simple DroneApp
{
	parameters:
	    string key = default(""); //chave de autenticação do drone 
        string destAddress = default("ff:ff:ff:ff:ff:ff");  // destination MAC address, or module path name of destination station; empty means off
        double startTime @unit(s) = default(this.sendInterval);  // time of sending the first request
        double stopTime @unit(s) = default(-1s);  // time of finishing sending, negative values mean forever
        int localSAP = default(0xf0);
        int remoteSAP = default(0xf1);
        volatile double sendInterval @unit(s) = default(uniform(0s,1s));  // interval between sending requests
        volatile int reqLength @unit(B) = default(100B);  // length of request packets
        volatile int respLength @unit(B) = default(1KiB);  // length of response packets
        bool registerSAP = default(false);   // whether to sent IEEE802CTRL_REGISTER_DSAP on startup
        @display("i=block/app");
        @signal[sentPk](type=cPacket);
        @signal[rcvdPk](type=cPacket);
        @statistic[rcvdPk](title="packets received"; source=rcvdPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[sentPk](title="packets sent"; source=sentPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[endToEndDelay](title="end-to-end delay"; source="messageAge(rcvdPk)"; unit=s; record=histogram,vector; interpolationmode=none);
    gates:
        input in @labels(Ieee802Ctrl/up);
        output out @labels(Ieee802Ctrl/down);
}
