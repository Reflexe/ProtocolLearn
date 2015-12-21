pl_add_library(Tcp TcpDataStream TcpFilter TcpPacket TcpServer TcpStream TcpOptionsParser TcpSequenceNumber)

include ( ../../pl_protocol.cmake )

pl_add_module_auto_caller(Utilities/Random)
pl_add_module_auto_caller(Utilities/NetworkFunctions)
pl_add_module_auto_caller(Utilities/TcpIpv4OptionsParser)
pl_add_module_auto_caller(Utilities/PacketStreamUnderPacketStream)

pl_add_module_auto_caller(Layers/Internet/IPProtocol)
pl_add_module_auto_caller(Layers/Transport/TransportProtocol)
