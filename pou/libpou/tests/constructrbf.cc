#include "ConstraintSet.h"
#include "ConstructRBF.h"
#include "ConstructRBFBiHarmonic.h"
#include "ConstructRBFTriHarmonic.h"
#include "ConstructRBFThinPlate.h"

int main(int argc, char *argv[]) {
  ConstructRBF *c = new ConstructRBFTriHarmonic();
  ConstructRBF *c2 = new ConstructRBFTriHarmonic();
  ConstraintSet cs;
  ConstraintSet cs2;

  cs.add(new Constraint(Vec3f(1,1,1), 5));
  cs.add(new Constraint(Vec3f(1,2,2), 3));
  cs.add(new Constraint(Vec3f(2,5,3), 10));
  cs.add(new Constraint(Vec3f(4,1,2), 8));
  cs.add(new Constraint(Vec3f(6,5,9), 3));
  cs.add(new Constraint(Vec3f(2,1,1), 5));
  cs.add(new Constraint(Vec3f(2,2,2), 3));
  cs.add(new Constraint(Vec3f(1,5,3), 10));
  cs.add(new Constraint(Vec3f(2,1,2), 8));
  cs.add(new Constraint(Vec3f(2,5,9), 3));

  cs2.add(new Constraint(Vec3f(1,1,1), 5));
  cs2.add(new Constraint(Vec3f(1,2,2), 3));
  cs2.add(new Constraint(Vec3f(2,5,3), 10));
  cs2.add(new Constraint(Vec3f(4,1,2), 8));
  cs2.add(new Constraint(Vec3f(6,5,9), 3));
  cs2.add(new Constraint(Vec3f(2,1,1), 5));
  cs2.add(new Constraint(Vec3f(2,2,2), 3));
  cs2.add(new Constraint(Vec3f(1,5,3), 10));
  cs2.add(new Constraint(Vec3f(2,1,2), 8));
  cs2.add(new Constraint(Vec3f(2,5,9), 3));

  std::cout << "TriHarmonic" << std::endl;
  c->compute(cs);
  std::cout << c->eval(Vec3f(1,1,1)) << "(5)\n";
  std::cout << c->eval(Vec3f(1,2,2)) << "(3)\n";
  std::cout << c->eval(Vec3f(2,5,3)) << "(10)\n";
  std::cout << c->eval(Vec3f(4,1,2)) << "(8)\n";
  std::cout << c->eval(Vec3f(6,5,9)) << "(3)\n";
  delete c;

  std::cout << "TriHarmonic" << std::endl;
  c2->compute(cs2);
  std::cout << c2->eval(Vec3f(1,1,1)) << "(5)\n";
  std::cout << c2->eval(Vec3f(1,2,2)) << "(3)\n";
  std::cout << c2->eval(Vec3f(2,5,3)) << "(10)\n";
  std::cout << c2->eval(Vec3f(4,1,2)) << "(8)\n";
  std::cout << c2->eval(Vec3f(6,5,9)) << "(3)\n";
  delete c2;

  return 0;
}

