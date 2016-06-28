# check_ib

This is used for checking a InfiniBand network
## Netfile
```
nodes:
  - guid: 0x8f10403991480
    name: mon.sos.ethz.ch
    isSM: true
links:
  - from: mon.sos.ethz.ch
    to: sw-ib-voltaire
```
i.e. list every node with it's GUID (ibstat, careful, not the port GUID) and list all links
of your network. Multilinks (i.e. two or more connections between the same hosts) only
need to be listed once. If a node is supposed to be a subnet manager, you have to add
"isSM: true" to it.

## Usage
```
# ./check_ib
One of check-host and check-net must be specified!
Options:
  --help                        print this message
  --check-host arg              check IB connectivity of a given host
  --netfile arg (=testfile.yml) location of the netfile (default: testfile.yml)
  --check-net                   check overall IB network status
  --clear                       clear performance counters after read
                                (recommended)
  --dump                        dump all detected information about the network
```
The check operates in two modes:
* Host check: Checks a single host. This verifies that all links of the host are present
  as expected and reads the performance counters of each port.
* Net check: Checks the network as a whole: Currently this only verifies that all listed
  subnet managers are present and reachable

## Example output
### Net check
```
# ./check_ib --check-net
OK | 'SM ceph-a.sos.ethz.ch-1 ACT'=99581c 'SM ceph-a.sos.ethz.ch-1 priority'=0 'SM ceph-a.sos.ethz.ch-1 state'=3 'SM ceph-a.sos.ethz.ch-2 ACT'=22382c 'SM ceph-a.sos.ethz.ch-2 priority'=0 'SM ceph-a.sos.ethz.ch-2 state'=2 'SM ceph-c.sos.ethz.ch-1 ACT'=464878c 'SM ceph-c.sos.ethz.ch-1 priority'=0 'SM ceph-c.sos.ethz.ch-1 state'=2 'SM ceph-c.sos.ethz.ch-2 ACT'=393382c 'SM ceph-c.sos.ethz.ch-2 priority'=0 'SM ceph-c.sos.ethz.ch-2 state'=2 'SM ceph-b.sos.ethz.ch-1 ACT'=136667c 'SM ceph-b.sos.ethz.ch-1 priority'=0 'SM ceph-b.sos.ethz.ch-1 state'=2 'SM ceph-b.sos.ethz.ch-2 ACT'=136644c 'SM ceph-b.sos.ethz.ch-2 priority'=0 'SM ceph-b.sos.ethz.ch-2 state'=2
# echo $?
0
```
### Host check(verbose)
```
# ./check_ib --check-host=ceph-a.sos.ethz.ch --dump
----------------------------------------------------------
Host: sw-ib-voltaire
	GUID: 8f1040041a7e6
	Number of ports: 24
UC: 4, Val: Port GUID 8f1040041a7e6 LID 7, port number 0
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::SDR(IBPort::LinkSpeed::SDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 1
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 1
		IBPort::PortAttribute::LINK_DOWN_COUNT: 1
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 1
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 204195712
		IBPort::PortAttribute::TX_PACKETS: 111270171
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 2c90200289562
		 Host: comp-1.sos.ethz.ch

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 2
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 3685741778
		IBPort::PortAttribute::RX_PACKETS: 97925679
		IBPort::PortAttribute::TX_PACKETS: 46057280
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 2c9020028951e
		 Host: comp-2.sos.ethz.ch

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 3
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 451785339
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 21684501
		IBPort::PortAttribute::TX_PACKETS: 54514505
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 2c902002896d2
		 Host: comp-3.sos.ethz.ch

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 4
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 5
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 6
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 7
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 8
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 9
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 10
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 2324143640
		IBPort::PortAttribute::TX_PACKETS: 2204222085
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040399124e
		 Host: ceph-a.sos.ethz.ch

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 11
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 65535
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 1
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 1763797888
		IBPort::PortAttribute::TX_PACKETS: 1910934913
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f104039941aa
		 Host: ceph-b.sos.ethz.ch

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 12
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 1540366001
		IBPort::PortAttribute::TX_PACKETS: 1681313303
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040399142a
		 Host: ceph-c.sos.ethz.ch

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 13
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 14
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 15
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 16
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 17
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 18
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 104184
		IBPort::PortAttribute::TX_BYTES: 105336
		IBPort::PortAttribute::RX_PACKETS: 1447
		IBPort::PortAttribute::TX_PACKETS: 1463
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f10403991482
		 Host: mon.sos.ethz.ch

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 19
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 221412078
		IBPort::PortAttribute::TX_PACKETS: 491696012
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: b8cffff005128
		 Host: sw-ib-mellanox

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 20
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 629836101
		IBPort::PortAttribute::TX_PACKETS: 304574157
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: b8cffff005128
		 Host: sw-ib-mellanox

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 21
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 22
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 23
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID 8f1040041a7e6 LID 7, port number 24
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0


----------------------------------------------------------
Host: comp-3.sos.ethz.ch
	GUID: 2c902002896d0
	Number of ports: 2
UC: 4, Val: Port GUID 2c902002896d1 LID 10, port number 1
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 3781539069
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 64149796
		IBPort::PortAttribute::TX_PACKETS: 174769942
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: b8cffff005128
		 Host: sw-ib-mellanox

UC: 4, Val: Port GUID 2c902002896d2 LID 6, port number 2
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 223614
		IBPort::PortAttribute::TX_BYTES: 98640
		IBPort::PortAttribute::RX_PACKETS: 5167
		IBPort::PortAttribute::TX_PACKETS: 1370
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040041a7e6
		 Host: sw-ib-voltaire


----------------------------------------------------------
Host: comp-2.sos.ethz.ch
	GUID: 2c9020028951c
	Number of ports: 2
UC: 4, Val: Port GUID 2c9020028951d LID 9, port number 1
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 1571474535
		IBPort::PortAttribute::TX_BYTES: 247596598
		IBPort::PortAttribute::RX_PACKETS: 13583208
		IBPort::PortAttribute::TX_PACKETS: 11484319
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: b8cffff005128
		 Host: sw-ib-mellanox

UC: 4, Val: Port GUID 2c9020028951e LID 3, port number 2
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 217494
		IBPort::PortAttribute::TX_BYTES: 96624
		IBPort::PortAttribute::RX_PACKETS: 5017
		IBPort::PortAttribute::TX_PACKETS: 1342
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040041a7e6
		 Host: sw-ib-voltaire


----------------------------------------------------------
Host: ceph-c.sos.ethz.ch
	GUID: 8f10403991428
	Number of ports: 2
UC: 4, Val: Port GUID 8f10403991429 LID 1, port number 1
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::SDR(IBPort::LinkSpeed::SDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 33666856
		IBPort::PortAttribute::TX_BYTES: 33586920
		IBPort::PortAttribute::RX_PACKETS: 468830
		IBPort::PortAttribute::TX_PACKETS: 466485
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: b8cffff005128
		 Host: sw-ib-mellanox

UC: 4, Val: Port GUID 8f1040399142a LID 4, port number 2
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::SDR(IBPort::LinkSpeed::SDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 525050818
		IBPort::PortAttribute::TX_PACKETS: 452204718
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040041a7e6
		 Host: sw-ib-voltaire


----------------------------------------------------------
Host: comp-1.sos.ethz.ch
	GUID: 2c90200289560
	Number of ports: 2
UC: 4, Val: Port GUID 2c90200289561 LID 13, port number 1
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 1
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 542933318
		IBPort::PortAttribute::TX_BYTES: 85708604
		IBPort::PortAttribute::RX_PACKETS: 4710837
		IBPort::PortAttribute::TX_PACKETS: 3986612
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: b8cffff005128
		 Host: sw-ib-mellanox

UC: 4, Val: Port GUID 2c90200289562 LID 12, port number 2
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 1
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 54893
		IBPort::PortAttribute::TX_BYTES: 55152
		IBPort::PortAttribute::RX_PACKETS: 765
		IBPort::PortAttribute::TX_PACKETS: 766
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040041a7e6
		 Host: sw-ib-voltaire


----------------------------------------------------------
Host: ceph-b.sos.ethz.ch
	GUID: 8f104039941a8
	Number of ports: 2
UC: 4, Val: Port GUID 8f104039941a9 LID 2, port number 1
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::SDR(IBPort::LinkSpeed::SDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 9938930
		IBPort::PortAttribute::TX_BYTES: 9938520
		IBPort::PortAttribute::RX_PACKETS: 138046
		IBPort::PortAttribute::TX_PACKETS: 138035
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: b8cffff005128
		 Host: sw-ib-mellanox

UC: 4, Val: Port GUID 8f104039941aa LID 14, port number 2
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::SDR(IBPort::LinkSpeed::SDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 276892092
		IBPort::PortAttribute::TX_PACKETS: 244429061
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040041a7e6
		 Host: sw-ib-voltaire


----------------------------------------------------------
Host: ceph-a.sos.ethz.ch
	GUID: 8f1040399124c
	Number of ports: 2
UC: 4, Val: Port GUID 8f1040399124d LID 8, port number 1
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::SDR(IBPort::LinkSpeed::SDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 11746656
		IBPort::PortAttribute::TX_BYTES: 11738016
		IBPort::PortAttribute::RX_PACKETS: 163148
		IBPort::PortAttribute::TX_PACKETS: 163028
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: b8cffff005128
		 Host: sw-ib-mellanox

UC: 4, Val: Port GUID 8f1040399124e LID 5, port number 2
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::SDR(IBPort::LinkSpeed::SDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 73500942
		IBPort::PortAttribute::TX_PACKETS: 75986424
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040041a7e6
		 Host: sw-ib-voltaire


----------------------------------------------------------
Host: sw-ib-mellanox
	GUID: b8cffff005128
	Number of ports: 24
UC: 4, Val: Port GUID b8cffff005128 LID 11, port number 0
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::SDR(IBPort::LinkSpeed::SDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 1
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 2
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 3
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 4
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 5
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 6
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 7
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 8
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 9
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 10
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 11
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 12
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 13
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 181872504
		IBPort::PortAttribute::TX_BYTES: 182046534
		IBPort::PortAttribute::RX_PACKETS: 2526007
		IBPort::PortAttribute::TX_PACKETS: 2530794
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040399124d
		 Host: ceph-a.sos.ethz.ch

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 14
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 65535
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 1
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 61014672
		IBPort::PortAttribute::TX_BYTES: 61149783
		IBPort::PortAttribute::RX_PACKETS: 847426
		IBPort::PortAttribute::TX_PACKETS: 851554
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f104039941a9
		 Host: ceph-b.sos.ethz.ch

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 15
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 66818088
		IBPort::PortAttribute::TX_BYTES: 66949174
		IBPort::PortAttribute::RX_PACKETS: 928029
		IBPort::PortAttribute::TX_PACKETS: 931910
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f10403991429
		 Host: ceph-c.sos.ethz.ch

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 16
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 1
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 310068781
		IBPort::PortAttribute::TX_BYTES: 1963722127
		IBPort::PortAttribute::RX_PACKETS: 14431750
		IBPort::PortAttribute::TX_PACKETS: 17054608
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 2c90200289561
		 Host: comp-1.sos.ethz.ch

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 17
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 179475019
		IBPort::PortAttribute::TX_PACKETS: 81360625
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 2c9020028951d
		 Host: comp-2.sos.ethz.ch

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 18
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 187422055
		IBPort::PortAttribute::TX_PACKETS: 79114391
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 2c902002896d1
		 Host: comp-3.sos.ethz.ch

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 19
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 84011125
		IBPort::PortAttribute::TX_PACKETS: 121195946
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040041a7e6
		 Host: sw-ib-voltaire

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 20
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::DDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4294967295
		IBPort::PortAttribute::TX_BYTES: 4294967295
		IBPort::PortAttribute::RX_PACKETS: 96273256
		IBPort::PortAttribute::TX_PACKETS: 262895712
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040041a7e6
		 Host: sw-ib-voltaire

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 21
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 22
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 0
		IBPort::PortAttribute::TX_BYTES: 0
		IBPort::PortAttribute::RX_PACKETS: 0
		IBPort::PortAttribute::TX_PACKETS: 0
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 23
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::POLLING
		Logical link state: IBPort::LogPortState::DOWN
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 288
		IBPort::PortAttribute::TX_BYTES: 288
		IBPort::PortAttribute::RX_PACKETS: 4
		IBPort::PortAttribute::TX_PACKETS: 4
		IBPort::PortAttribute::TX_WAIT_TIME: 0

UC: 3, Val: Port GUID b8cffff005128 LID 11, port number 24
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::DDR(IBPort::LinkSpeed::DDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 4129992
		IBPort::PortAttribute::TX_BYTES: 4124016
		IBPort::PortAttribute::RX_PACKETS: 57361
		IBPort::PortAttribute::TX_PACKETS: 57278
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f10403991481
		 Host: mon.sos.ethz.ch


----------------------------------------------------------
Host: mon.sos.ethz.ch
	GUID: 8f10403991480
	Number of ports: 2
UC: 4, Val: Port GUID 8f10403991481 LID 15, port number 1
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::SDR(IBPort::LinkSpeed::SDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 1940904
		IBPort::PortAttribute::TX_BYTES: 1948536
		IBPort::PortAttribute::RX_PACKETS: 26957
		IBPort::PortAttribute::TX_PACKETS: 27063
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: b8cffff005128
		 Host: sw-ib-mellanox

UC: 4, Val: Port GUID 8f10403991482 LID 16, port number 2
	 Link:
		Width IBPort::LinkWidth::LW_4 out of IBPort::LinkWidth::LW_4(IBPort::LinkWidth::LW_4 enabled)
		Rate IBPort::LinkSpeed::SDR out of IBPort::LinkSpeed::SDR(IBPort::LinkSpeed::SDR enabled)
		Physical link state: IBPort::PHYSPortState::LINK_UP
		Logical link state: IBPort::LogPortState::ACTIVE
	 Port attributes:
		IBPort::PortAttribute::SYMBOL_ERRORS: 0
		IBPort::PortAttribute::LINK_ERRS_RECOVERED: 0
		IBPort::PortAttribute::LINK_DOWN_COUNT: 0
		IBPort::PortAttribute::PORT_RX_ERRS: 0
		IBPort::PortAttribute::PORT_TX_DISCARDED: 0
		IBPort::PortAttribute::PORT_TX_CONSTR_ERRS: 0
		IBPort::PortAttribute::PORT_RX_CONSTR_ERRS: 0
		IBPort::PortAttribute::LOCAL_LINK_INTEGR_ERRS: 0
		IBPort::PortAttribute::EXCESSIVE_BUFFER_OVERRUNS: 0
		IBPort::PortAttribute::RX_BYTES: 39096
		IBPort::PortAttribute::TX_BYTES: 39096
		IBPort::PortAttribute::RX_PACKETS: 543
		IBPort::PortAttribute::TX_PACKETS: 543
		IBPort::PortAttribute::TX_WAIT_TIME: 0
	 Peer port:
		 GUID: 8f1040041a7e6
		 Host: sw-ib-voltaire


OK | 'port 1 symbol errors'=0 'port 1 recovered link errors'=0 'port 1 unrecovered link errors'=0 'port 1 RX packets with error'=0 'port 1 TX packets discarded'=0 'port 1 invalid TX packet constraints'=0 'port 1 invalid RX packet constraints'=0 'port 1 local link integrity errors'=0 'port 1 buffer overruns'=0 'port 1 RX (bytes)'=11746656B 'port 1 TX (bytes)'=11738016B 'port 1 RX (packets)'=163148 'port 1 TX (packets)'=163028 'port 1 average transmit wait time'=0 'port 2 symbol errors'=0 'port 2 recovered link errors'=0 'port 2 unrecovered link errors'=0 'port 2 RX packets with error'=0 'port 2 TX packets discarded'=0 'port 2 invalid TX packet constraints'=0 'port 2 invalid RX packet constraints'=0 'port 2 local link integrity errors'=0 'port 2 buffer overruns'=0 'port 2 RX (bytes)'=4294967295B 'port 2 TX (bytes)'=4294967295B 'port 2 RX (packets)'=73500942 'port 2 TX (packets)'=75986424 'port 2 average transmit wait time'=0
```