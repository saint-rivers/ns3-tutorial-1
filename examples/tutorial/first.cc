/*
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

int
main(int argc, char* argv[])
{
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NodeContainer nodes;
    nodes.Create(2);

    // configure point-to-point attributes of the connection between two points
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer devices;
    // this installs point-to-point connection capabilities onto the nodes
    devices = pointToPoint.Install(nodes);
    /**
     * After executing the pointToPoint.Install(nodes) call we will have two nodes, each with an
     * installed point-to-point net device and a single point-to-point channel between them. Both
     * devices will be configured to transmit data at five megabits per second over the channel
     * which has a two millisecond transmission delay.
     */

    InternetStackHelper stack;
    // this installs internet connection capabilities onto the nodes
    stack.Install(nodes);

    // sets the network address that IP addresses will be doled out to node devices
    // it will be managed and auto assigned from the network address
    // This will throw an error if two nodes are incidentally assigned the same address
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");

    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    // echoServer(...) is a parameterized constructor
    UdpEchoServerHelper echoServer(9);

    // install echo application onto the second node (node[1])
    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
    serverApps.Start(Seconds(1.0)); // schedule to start at 1 second into the simulation
    serverApps.Stop(Seconds(10.0)); // schedule to start at 10 seconds into the simulation

    // send to ipAddr=address[1], port=9
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval",
                            TimeValue(Seconds(1.0))); // how long to wait in between packets
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
