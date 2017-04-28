package vant_wifi.simulations;

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import vant_wifi.node.Drone;
import vant_wifi.node.Radar;


@license(LGPL);

network Industry
{
    parameters:
        @display("bgb=500,500");
        int numDrones;

    submodules:
        radar1: Radar {
            parameters:
                @display("i=misc/sensor;is=s;p=82,67");
        }

        drone[numDrones]: Drone {
            parameters:
                @display("i=misc/node;is=s");
        }

        radioMedium: Ieee80211ScalarRadioMedium {
            parameters:
                @display("p=275,23;is=s");
        }

        // Aparentemente não precisa configurar rede IPv4
        configurator: IPv4NetworkConfigurator {
            @display("p=368,23;is=s");
        }
}
