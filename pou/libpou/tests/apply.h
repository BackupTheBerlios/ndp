#ifndef APPLY_H
#define APPLY_H

template<typename Iterator, typename InputIterator, typename Function>
void for_each_binary(Iterator first, Iterator last,
                     InputIterator firstArg, Function f)
{
  while( first != last )
    f(*first++, *firstArg++);
}

template<typename ContainSource, typename ContainSourceOSG,
         typename ContainSource2, typename ContainSource2OSG,
         typename Contain, typename ContainOSG, typename Predicate>
void checkBinaryError(ContainSource &source, ContainSourceOSG &sourceOSG,
                      ContainSource2 &source2, ContainSource2OSG &source2OSG,
                      Contain res, ContainOSG resOSG, Predicate eq)
{
  using namespace std;
  pair<typename Contain::iterator, typename ContainOSG::iterator> p;
  p = mismatch(res.begin(), res.end(), resOSG.begin(), eq);
  if(p.first == res.end())
    cout << " OK" << endl;
  else {
    cout << "***Error***" << endl;
    int i = distance(res.begin(), p.first);
    cout <<"MATH: (" << source[i] << ") (" << source2[i] << ") = " << fixed
         << *p.first << resetiosflags(ios::floatfield)  << endl;
    cout << "OSG: (" << sourceOSG[i] << ") (" << source2OSG[i] << ") = "
         << fixed << *p.second << resetiosflags(ios::floatfield) << endl;
  }
}

template<typename ContainSource, typename ContainSourceOSG,
         typename Contain, typename ContainOSG, typename Predicate>
void checkUnaryError(ContainSource &source, ContainSourceOSG &sourceOSG,
                     Contain res, ContainOSG resOSG, Predicate eq)
{
  using namespace std;
  pair<typename Contain::iterator, typename ContainOSG::iterator> p;
  p = mismatch(res.begin(), res.end(), resOSG.begin(), eq);
  if(p.first == res.end())
    cout << " OK" << endl;
  else {
    cout << "***Error" << endl;
    int i = distance(res.begin(), p.first);
    cout <<"(" << source[i] << ") = " << fixed << *p.first
         << resetiosflags(ios::floatfield)  << endl;
    cout <<"(" << sourceOSG[i] << ") = " << fixed << *p.second
         << resetiosflags(ios::floatfield)  << endl;
  }
}

/*------------------------------*/

/*1 argument and non void return value*/
template<typename UnaryFunction1, typename UnaryFunction2,
         typename ContainSource1, typename ContainSource2,
         typename ContainDest, typename ContainDestOSG,
         typename BinaryPredicate>
void testUnary(ContainSource1 &source, ContainSource2 &sourceOSG,
               ContainDest &dest, ContainDestOSG &destOSG,
               UnaryFunction1 f, UnaryFunction2 fOSG, BinaryPredicate eq)
{
  using namespace std;
  transform(source.begin(), source.end(), dest.begin(), f);
  transform(sourceOSG.begin(), sourceOSG.end(), destOSG.begin(), fOSG);
  checkUnaryError(source, sourceOSG, dest, destOSG, eq);
}

/*1 argument and void return value*/
template<typename UnaryFunction1, typename UnaryFunction2,
         typename ContainSource, typename ContainSourceOSG,
         typename BinaryPredicate>
void testApply(ContainSource &source, ContainSourceOSG &sourceOSG,
               UnaryFunction1 f, UnaryFunction2 fOSG, BinaryPredicate eq)
{
  using namespace std;
  ContainSource tmp(source.size());
  ContainSourceOSG tmpOSG(sourceOSG.size());
  pair<typename ContainSource::iterator, typename ContainSourceOSG::iterator> p;
  copy(source.begin(), source.end(), tmp.begin());
  copy(sourceOSG.begin(), sourceOSG.end(), tmpOSG.begin());
  for_each(tmp.begin(), tmp.end(), f);
  for_each(tmpOSG.begin(), tmpOSG.end(), fOSG);
  checkUnaryError(source, sourceOSG, tmp, tmpOSG, eq);
}

/*2 arguments and non void return value*/
template<typename BinaryFunction1, typename BinaryFunction2,
         typename ContainSource, typename ContainSourceOSG,
         typename ContainSource2, typename ContainSource2OSG,
         typename ContainDest, typename ContainDestOSG,
         typename BinaryPredicate>
void testBinary(ContainSource &source, ContainSourceOSG &sourceOSG,
                ContainSource2 &source2, ContainSource2OSG &source2OSG,
                ContainDest &dest, ContainDestOSG &destOSG,
                BinaryFunction1 f, BinaryFunction2 fOSG, BinaryPredicate eq)
{
  using namespace std;
  transform(source.begin(), source.end(), source2.begin(), dest.begin(), f);
  transform(sourceOSG.begin(), sourceOSG.end(), source2OSG.begin(), destOSG.begin(), fOSG);
  checkBinaryError(source, sourceOSG, source2, source2OSG, dest, destOSG, eq);
}

/*2 arguments and void return value*/
template<typename BinaryFunction1, typename BinaryFunction2,
         typename ContainSource, typename ContainSourceOSG,
         typename ContainSource2, typename ContainSource2OSG,
         typename BinaryPredicate>
void testBinary(ContainSource &source, ContainSourceOSG &sourceOSG,
                ContainSource2 &source2, ContainSource2OSG &source2OSG,
                BinaryFunction1 f, BinaryFunction2 fOSG, BinaryPredicate eq)
{
  using namespace std;
  ContainSource tmp(source.size());
  ContainSourceOSG tmpOSG(sourceOSG.size());

  copy(source.begin(), source.end(), tmp.begin());
  copy(sourceOSG.begin(), sourceOSG.end(), tmpOSG.begin());
  for_each_binary(tmp.begin(), tmp.end(), source2.begin(), f);
  for_each_binary(tmpOSG.begin(), tmpOSG.end(), source2OSG.begin(), fOSG);
  checkBinaryError(source, sourceOSG, source2, source2OSG, tmp, tmpOSG, eq);
}

/*2 arguments, void return value and result into second argument */
template<typename BinaryFunction1, typename BinaryFunction2,
         typename ContainSource, typename ContainSourceOSG,
         typename ContainDest, typename ContainDestOSG,
         typename BinaryPredicate>
void testBinarySecond(ContainSource &source, ContainSourceOSG &sourceOSG,
                      ContainDest &dest, ContainDestOSG &destOSG,
                      BinaryFunction1 f, BinaryFunction2 fOSG,
                      BinaryPredicate eq)
{
  using namespace std;

  for_each_binary(source.begin(), source.end(), dest.begin(), f);
  for_each_binary(sourceOSG.begin(), sourceOSG.end(), destOSG.begin(), fOSG);
  checkUnaryError(source, sourceOSG, dest, destOSG, eq);
}

#endif
