// Example from Modelica Spec issue #2263 about v3.4 lookup in instance vs lexical scope
// https://github.com/modelica/ModelicaSpecification/issues/2263#issuecomment-435715211

model M

record R
    Real x,y;
end R;

package B
   type R = Real[3];
   model M
      R x;
   end M;
end B;

B.M m;

end M;

// OMC 1.25.0 -i=M gives:
// class M
//   Real m.x[1];
//   Real m.x[2];
//   Real m.x[3];
// end M;

// OMC result is agreed (but as `Real[3] m.x`) in comment by Elena Shmoylova (MapleSim)
// https://github.com/modelica/ModelicaSpecification/issues/2263#issuecomment-435715317
