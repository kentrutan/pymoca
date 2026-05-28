package P
  connector RealOutput = output Real;

  expandable connector Bus
    RealOutput speed;
  end Bus;

  model Sender
    Bus bus;
  end Sender;

  model System
    Bus bus;
    Sender s;
  equation
    connect(s.bus, bus);
  end System;
end P;
