
void Test() {

  TClass *cls = TClass::GetClass("TProfile");
  
  for( TIter next(cls->GetListOfAllPublicMethods()); TMethod *m = (TMethod*)next(); ) {
    TString name = m->GetName();
    cout << m->GetPrototype() << ", " << m->GetMangledName() << endl;
  }
}
