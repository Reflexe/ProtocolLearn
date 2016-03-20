pl_add_library(DNS DnsPacket DnsStream DnsFilter)

include ( ../../pl_protocol.cmake )

pl_add_module_auto_caller(Utilities/BitField)
