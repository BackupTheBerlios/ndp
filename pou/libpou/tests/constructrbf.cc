#include "ConstructRBF.h"
#include "ConstructRBFBiHarmonic.h"
#include "ConstructRBFTriHarmonic.h"

int main(int argc, char *argv[]) {
  ConstructRBF *c = new ConstructRBFTriHarmonic();
  ConstructRBF *c1 = new ConstructRBFBiHarmonic();
  std::vector<Constraint> cs;

  cs.push_back(Constraint(Vec3f(1,1,1), 5));
  cs.push_back(Constraint(Vec3f(1,2,2), 3));
  cs.push_back(Constraint(Vec3f(2,5,3), 10));
  cs.push_back(Constraint(Vec3f(4,1,2), 8));
  cs.push_back(Constraint(Vec3f(6,5,9), 3));

  c->compute(cs);
  c1->compute(cs);
  
  for (double i=0; i<10; i+=0.1) {
    Vec3f v(i,i,i);
    Vec3f r;
    std::cout << "c(" << v << ") = " << c->eval(v) << std::endl;
    std::cout << "  gradian (" << (c-> evalGradian(v, r),r) << ")\n  normal (" << (c->evalNormal(v, r),r) << ")" << std::endl;
  }
  delete c;

  c=c1;
  for (double i=0; i<10; i+=0.1) {
    Vec3f v(i,i,i);
    Vec3f r;
    std::cout << "c(" << v << ") = " << c->eval(v) << std::endl;
    std::cout << "  gradian(" << (c-> evalGradian(v, r),r) << ")\n  normal(" << (c->evalNormal(v, r),r) << ")" << std::endl;
  }
  delete c;
  
}

