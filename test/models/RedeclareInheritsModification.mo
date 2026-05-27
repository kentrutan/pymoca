// Regression test for the state-synced stub bug:
// A replaceable symbol is declared with a parameter modifier (final m=m).
// A subclass redeclares it to an ExtendedPort that extends PartialPort.
// PartialPort has 'm' as a direct parameter.  When flattening the example,
// the instantiator must be able to apply the inherited 'm=m' modifier even
// though the instantiated InstanceClass for PartialPort was temporarily
// state-synced to FULL with empty symbols (the "lexical stub" bug).
package P
  connector PartialPort
    parameter Integer m = 3;
    Real v[m];
  end PartialPort;

  connector ExtendedPort
    extends PartialPort;
    parameter Boolean flag = false;
  end ExtendedPort;

  model MachineBase
    parameter Integer m = 3;
    replaceable PartialPort port(final m = m);
  end MachineBase;

  model Machine
    extends MachineBase(
      redeclare final ExtendedPort port(final flag = true)
    );
  end Machine;

  model Example
    Machine mac(m = 2);
  end Example;
end P;
