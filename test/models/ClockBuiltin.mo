package ClockBuiltin
  connector ClockInput = input Clock;
  connector ClockOutput = output Clock;

  model UsesClock
    ClockInput u;
    ClockOutput y;
  end UsesClock;
end ClockBuiltin;
