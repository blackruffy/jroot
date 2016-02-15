/*
Copyright (c) 2016, Taichi Kubo
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "TMap.h"
#include "TList.h"
#include "THashTable.h"
#include "TString.h"
#include "TRegexp.h"
#include "TObjString.h"
#include "TClass.h"
#include "TMethod.h"
#include "TMethodArg.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <string>

using namespace std;

////////////////////////////////////////////////////////////
// 便利関数
////////////////////////////////////////////////////////////

template<class T> class Pair {
public:
  const T _1;
  const T _2;
  Pair( const T x1, const T x2 ):
    _1(x1),
    _2(x2){}
};

class JNIArg {
public:
  TString _1;
  TString _2;
  TString _3;
  JNIArg():
    _1(""),
    _2(""),
    _3(""){};
  
  JNIArg(const TString x1,
          const TString x2,
          const TString x3 ):
    _1(x1),
    _2(x2),
    _3(x3){};

  void append(const JNIArg& t) {
    _1 += t._1;
    _2 += t._2;
    if( _3 != "" ) _3 += ", ";
    _3 += t._3;
  };
  
  void set(const JNIArg& t) {
    _1 = t._1;
    _2 = t._2;
    _3 = t._3;
  };
};

class Arg : public TObject {
public:
  const TString type;
  const TString name;
  const TString orgType;
  Arg( const TString &_type, const TString &_name, const TString &_orgType ):
    TObject(),
    type(_type),
    name(_name),
    orgType(_orgType){}
};

class TInt : public TObject {
public:
  int x;
  TInt( int x ) : TObject() {
    this->x = x;
  };
};

bool isUpperCase( char x ) {
  return x >= 'A' && x <= 'Z';
}

bool isLowerCase( char x ) {
  return x >= 'a' && x <= 'z';
}

TString toUpperCase( TString name ) {
  char x = name[0];
  if( isLowerCase( x ) ) name[0] = x - 32;
  return name;
}

/**
 * Java用のメソッド名に変換する。
 */
TString toLowerMethodName( const TString &_name ) {
  TString name = _name;
  char x = name[0];
  if( isUpperCase( x ) && !isUpperCase( name[1] ) ) name[0] = x + 32;
  else name[0] = x;
  return name;
}

TObjString* obj( const TString &str ) {
  return new TObjString( str );
}

void add( TMap *map, const TString &key, const TString &value ) {
  map->Add( obj( key ), obj( value ) );
}

void add( TMap *map, const TString &key ) {
  map->Add( obj( key ), obj( key ) );
}

void add( THashTable *set, const TString &key ) {
  set->Add( obj( key ) );
}

bool has( const TMap *map, const TString &key ) {
  TObjString *_key = obj( key );
  bool x = map->Contains( _key );
  delete _key;
  return x;
}

bool has( const THashTable *set, const TString &key ) {
  TObjString *_key = obj( key );
  bool x = set->Contains( _key );
  delete _key;
  return x;
}

TString get( const TMap *map, const TString &key ) {
  TObjString *_key = obj(key);
  TString x = ((TObjString*)map->GetValue( _key ))->GetString();
  delete _key;
  return x;
}

TString get( const THashTable *set, const TString &key ) {
  TObjString *_key = obj(key);
  TString x = ((TObjString*)set->FindObject( _key ))->GetString();
  delete _key;
  return x;
}

/**
 * ２つのTHashTableを連結して新しいTHashTableを返す。
 */
THashTable* concat( const THashTable *set1, const THashTable *set2 ) {
  THashTable *set = new THashTable();
  TObjString *key;
  
  TIter iter1( set1 );
  while( (key = (TObjString*)iter1()) ) {
    add( set, key->String() );
  }
  
  TIter iter2( set2 );
  while( (key = (TObjString*)iter2()) ) {
    add( set, key->String() );
  }

  return set;
}

/**
 * '*'や'&'や'const'を取り除く
 */
TString stripTypeName( const TString &name ) {
  TString retClassName = name;
  if( retClassName.EndsWith("&" ) )
    retClassName = retClassName.ReplaceAll("&", "");
  if( retClassName.EndsWith("*" ) )
    retClassName = retClassName.ReplaceAll("*", "");
  if( retClassName.EndsWith("&" ) )
    retClassName = retClassName.ReplaceAll("&", "");
  if( retClassName.BeginsWith("const") )
    retClassName = retClassName.ReplaceAll("const ", "");
  return retClassName;
}

/**
 * methodの引数の型の文字列をカンマ区切りで結合して返す。
 */
TString getMethodArgTypes( TMethod *method ) {
  TString name = "";
  TIter next( method->GetListOfMethodArgs() );
  TMethodArg *arg;
  while( (arg = (TMethodArg*)next()) ) {
    if( name != "" ) name += ", ";
    name += arg->GetFullTypeName();
  }
  return name;
}

bool isInvalidTypes( const THashTable *excludeTypes,
                     const TList *methodArgs ) {
  TIter next( methodArgs );
  TMethodArg *arg;
  bool flag = true;
  while( (arg = (TMethodArg*)next()) ) {
    if( has( excludeTypes, arg->GetTypeName() ) ) flag = false;
    else if( has( excludeTypes, arg->GetFullTypeName() ) ) flag = false;
  }
  return !flag;
}

bool hasInvalidType( const THashTable *excludeTypes,
                     TMethod *method )
{
  return isInvalidTypes( excludeTypes,
                         method->GetListOfMethodArgs() ) ||
    has( excludeTypes,
         stripTypeName(method->GetReturnTypeName()) ) ||
    has( excludeTypes,
         method->GetReturnTypeName() );
}

TString additionalHeaderMethod(TString &className)
{
  TString src = "";
  src += "  void " + className + "_destroy(addr_t self);\n";
  return src;
}

TString additionalCppMethod(TString &className) {
  TString src = "";
  src += "void " + className + "_destroy(addr_t self) {\n";
  src += "  delete reinterpret_cast<" + className + "*>(self);\n";
  src += "}\n";
  return src;
}

/**
 * @param otype ROOTでの型
 * @param ctype Cでの型
 * @param rval JAVAでの型
 */
TString convJNAArrayOrRef( const TString &otype,
                        const TString &ctype,
                        const TString &rval )
{
  TString tmp = otype;
  tmp.ReplaceAll("const ", "");
  return rval;
}

/**
 * @param otype ROOTでの型
 * @param ctype Cでの型
 * @param rval JAVAでの型
 */
TString convJavaArrayOrRef( const TString &otype,
                        const TString &ctype,
                        const TString &rval )
{
  TString tmp = otype;
  tmp.ReplaceAll("const ", "");
  if( ctype == "addr_t" ) return "Pointer";
  return rval;
}

TString convSpecialJNAType( const TString &className,
                            const TString &methodName,
                            const Arg *arg,
                            const TString &type )
{
  if( className == "TApplication" &&
      methodName == "TApplication" &&
      arg->name == "argc" &&
      arg->type == "int*") {
    return "long";
  }
  return type;
}

TString convSpecialJavaType( const TString &className,
                         const TString &methodName,
                         const Arg *arg,
                         const TString &type )
{
  if( className == "TApplication" &&
      methodName == "TApplication" &&
      arg->name == "argc" &&
      arg->type == "addr_t") {
    return "Pointer";
  }
  return type;
}

/**
 * Cラッパー関数内でROOT関数を呼び出す際に、
 * その関数の引数に渡す値を例外的にキャストする。
 */
TString convSpecialCType( const TString &className,
                         const TString &methodName,
                         const Arg *arg,
                         const TString &type )
{
  if( className == "TTree" &&
      methodName == "Branch" &&
      arg->name == "addobj" &&
      arg->type == "addr_t" ) {
    return TString::Format("reinterpret_cast<TObject*>(%s)",
                           arg->name.Data());
  }
  else if( className == "TApplication" &&
           methodName == "TApplication" &&
           arg->name == "argc" &&
           arg->type == "addr_t" ) {
    return TString::Format("reinterpret_cast<int*>(%s)",
                           arg->name.Data());
  }
  return type;
}

JNIArg convJNIReturnValue(TString jnicppRetrunType,
                          TString name)
{
  if( jnicppRetrunType == "jstring" ) {
    JNIArg rval;
    
    rval._1 += "jstring jni_"+name+" = env->NewStringUTF("+name+");\n";
    rval._3 += "jni_"+name;
    return rval;
  }
  else if( jnicppRetrunType == "jintArray" ) {
    throw TString::Format("unsupported JNI return type: %s at convJNIRetrunValue", jnicppRetrunType.Data());
  }
  else if( jnicppRetrunType == "jlongArray" ) {
    throw TString::Format("unsupported JNI return type: %s at convJNIRetrunValue", jnicppRetrunType.Data());
  }
  else if( jnicppRetrunType == "jfloatArray" ) {
    throw TString::Format("unsupported JNI return type: %s at convJNIRetrunValue", jnicppRetrunType.Data());
  }
  else if( jnicppRetrunType == "jdoubleArray" ) {
    throw TString::Format("unsupported JNI return type: %s at convJNIRetrunValue", jnicppRetrunType.Data());
  }
  else if( jnicppRetrunType == "jbyteArray" ) {
    throw TString::Format("unsupported JNI return type: %s at convJNIRetrunValue", jnicppRetrunType.Data());
  }
  else if( jnicppRetrunType == "jobjectArray" ) {
    throw TString::Format("unsupported JNI return type: %s at convJNIRetrunValue", jnicppRetrunType.Data());
  }
  else {
    return JNIArg("", "", name);
  }
  
}

JNIArg convJNIPrimArray(TString type1,
                         TString type2,
                         TString name )
{
  return JNIArg
    (TString("j") + type1+"* jni_"+name+" = env->Get"+type2+"ArrayElements("+name+", NULL);\n",
     TString("env->Release")+type2+"ArrayElements("+name+", jni_"+name+", 0);\n",
     TString("jni_") + name);
}

JNIArg convJNIArg(TMap* jniTypes,
                    TString jtype,
                    TString name)
{
  if( has(jniTypes, jtype) ) {
    TString jnitype = get(jniTypes, jtype);
    if( jnitype == "jstring" ) {
      JNIArg rval;
      rval._1 += "const char* tmp_"+name+" = env->GetStringUTFChars("+name+", NULL);\n";
      rval._1 += "char* jni_"+name+" = (char*)malloc(strlen(tmp_"+name+"));\n";
      rval._1 += "strcpy(jni_"+name+", tmp_"+name+");\n";
      rval._2 += "env->ReleaseStringUTFChars("+name+", tmp_"+name+");\n";
      rval._2 += "free(jni_"+name+");\n";
      rval._3 += "jni_" + name;
      return rval;
    }
    else if( jnitype == "jintArray" ) {
      return convJNIPrimArray("int", "Int", name);
    }
    else if( jnitype == "jlongArray" ) {
      return convJNIPrimArray("long", "Long", name);
    }
    else if( jnitype == "jfloatArray" ) {
      return convJNIPrimArray("float", "Float", name);
    }
    else if( jnitype == "jdoubleArray" ) {
      return convJNIPrimArray("double", "Double", name);
    }
    else if( jnitype == "jbyteArray" ) {
      return convJNIPrimArray("char", "Byte", name);
    }
    else if( jnitype == "jobjectArray" ) {
      JNIArg rval;
      rval._1 += "jsize len = env->GetArrayLength("+name+");\n";
      rval._1 += "char* jni_"+name+"[len];\n";
      rval._1 += "for( jsize i=0; i<len; i++ ) {\n";
      rval._1 += "  jstring jstr = (jstring) env->GetObjectArrayElement("+name+", i);\n";
      rval._1 += "  const char* tmp_"+name+" = env->GetStringUTFChars(jstr, NULL);\n";
      rval._1 += "  jni_"+name+"[i] = (char*)malloc(strlen(tmp_"+name+"));\n";
      rval._1 += "  env->ReleaseStringUTFChars(jstr, tmp_"+name+");\n";
      rval._1 += "}\n";
      rval._2 += "for( jsize i=0; i<len; i++ ) {\n";
      rval._2 += "  free(jni_"+name+"[i]);\n";
      rval._2 += "}\n";
      rval._3 += "jni_" + name;
      return rval;
    }
    else {
      return JNIArg("", "", name);
    }
  }
  else {
    throw TString::Format("failed to get jni arg type for %s at convJNIType", name.Data());
  }
}

/**
 * Cの型からJNAの型へ変換する。
 */
TString convJNAType( const TMap *javaTypes,
                     const TString &otype,
                     const TString &ctype )
{
  TString rval;
  if( has(javaTypes, ctype) ) {
    rval = get( javaTypes, ctype );
    rval = convJNAArrayOrRef(otype, ctype, rval);
  }
  else {
    throw TString("could not convert from c type to java type: ") + ctype + "(originaly " + otype + ") at convJNAType";
  }
  if( rval == "" ) {
    throw TString("empty return value: ") + ctype + "(originaly " + otype + ") at convJNAType";
    abort();
  }
  return rval;
};

/**
 * Cの型からJavaの型へ変換する。
 * @param imps ROOTの実装するクラスのテーブル
 * @param javaTypes CからJavaへの型変換テーブル
 * @param cType ROOTからCへの型変換テーブル
 * @param ctype 変換対象のCの型
 */
TString convJavaType( const THashTable *imps,
                      const TMap *javaTypes,
                      const TMap *cTypes,
                      const TString &ctype )
{
  TString _ctype = stripTypeName(ctype);
  // 実装予定のROOTのクラスだった場合
  if( has(imps, _ctype) )
    return _ctype;
  // 変換可能なCの型だった場合
  else if( has(cTypes, ctype) ) {
    TString dtype = get(cTypes, ctype);
    TString rval = get(javaTypes, dtype);
    return convJavaArrayOrRef(ctype, dtype, rval);
  }
  else {
    throw TString("[ERROR] Unsupported type at convJavaType: ") + ctype;
    return "";
  }
}

/**
 * C++やROOTの型をCの型に変換する。
 * ROOTのクラスは、void*などに変換される。
 * 無効な型の場合は、空の文字が返される。
 * @param cTypes Rootの型からCの型への変換テーブル
 * @param imps 実装するクラス・インターフェース一覧
 * @param excludeTypes 除外する型・クラス
 * @param type Rootの型
 */
TString convCType( const TMap *cTypes,
                   const THashTable *imps,
                   const THashTable *excludeTypes,
                   const TString &type ) {
  TString _type;
  // サポートするCの型に含まれている場合
  // Rootの型でCの型に変換できる型(プリミティブなど)
  if( has( cTypes, type ) ) {
    _type = get( cTypes, type );
  }
  // Cの型で表現できないRootの型・クラス
  else {
    TString tname = stripTypeName(type);
    // サポートするROOTの型に含まれている
    if( has( imps, tname ) ) {
      if( type.BeginsWith("const") )
        _type = "const ";
      return _type + "addr_t";
    }
    else if( !has( excludeTypes, tname ) ) {
      throw TString("Unexpected type: '") + tname + "', which is not in imps and excludeTypes or '" + type + "' is not in cTypes\n  at convCType()";
    }
  }
  return _type;
}

TString convMethodName( const TMap *operators,
                        const THashTable *excludeOperators,
                        const TString &methodName ) {
  TString _methodName;
  // 演算子の場合
  if( methodName.Contains("operator")
      || methodName == "New"
      || methodName == "Import"
      || methodName == "Notify" ) {
    if( has(operators, methodName) )
      _methodName = get(operators, methodName);
    else if( !has(excludeOperators, methodName) ) {
      throw TString::Format("unsupported operator: %s at convMethodName", methodName.Data());
    }
  }
  else if( methodName.Contains(TRegexp("<.+>")) ) {
    _methodName = "";
  }
  // デストラクタの場合
  else if( methodName.BeginsWith("~") ) {
    TString s = methodName;
    s.ReplaceAll("~", "");
    _methodName = "destruct" + s;
  }
  else _methodName = methodName;
  return toLowerMethodName(_methodName);
}

/**
 * リストのコピーを作成する。
 */
TList *clone( const TList *list ) {
  TList *resp = new TList();
  for( TIter next(list); TObject *o = next(); ) {
    resp->Add(o);
  }
  return resp;
}

/**
 * ROOT関数の引数の型からCラッパー関数の引数の型に変換する。
 * あるメソッドの引数のリストを受け取り、
 * それぞれの型を変換して、
 * 引数のリストにして返す。
 * デフォルト引数がない場合には、１つリストを返す。
 * デフォルト引数がある場合には、複数のリストを返す。
 * @param cTypes ROOTからCの型への変換規則
 * @param imps ROOTのクラスとインターフェースのテーブル
 * @param excludeTypes 除外する型・クラスのテーブル
 * @param methodArgs あるメソッドの引数のリスト
 */
TList *convArgTypes( const TMap *cTypes,
                     const THashTable *imps,
                     const THashTable *excludeTypes,
                     const TString className,
                     const TString methodName,
                     const TList *methodArgs ) {
  try {
    TIter next(methodArgs);
    TMethodArg *arg;
    TList *args = new TList(); // 引数のリスト
    TList *resp = new TList(); // オーバーロードリスト
    int cnt = 1; // 引数のカウント
    while( (arg = (TMethodArg*)next()) ) {
      TString orgType = arg->GetFullTypeName();
      TString type = convCType(cTypes,
                               imps,
                               excludeTypes,
                               orgType);
      TString name = arg->GetName();
      // 引数に名前が付けられてない場合
      if( name == "" ) name = TString::Format("arg%d", cnt);
      
      if( className == "TApplication" &&
          methodName == "TApplication" &&
          name == "argc" &&
          type == "int*" ) {
        type = "addr_t";
      }
      
      TString def = arg->GetDefault();
      // デフォルト引数が設定されている場合
      if( def != "" ) resp->Add( clone(args) );

      // 無効な型の場合は、そのメソッドは実装しない
      if( type == "" ) return resp;
      else {
        Arg *a = new Arg( type, name, orgType );
        args->Add( a );
      }
      
      cnt++;
    }
    resp->Add(args);
    return resp;
  } catch( TString msg ) {
    throw msg + "\n  at convArgTypes";
  }
}

/**
 * @param cTypes Rootの型からCの型への変換テーブル
 * @param imps 実装するクラス・インターフェース一覧
 * @param excludeTypes 除外する型・クラス
 * @param returnType Rootの型
 */
TString convReturnType( const TMap *cTypes,
                        const THashTable *imps,
                        const THashTable *excludeTypes,
                        TString returnType ) {
  try {
    TString r = convCType( cTypes,
                           imps,
                           excludeTypes,
                           returnType );
    if( returnType.BeginsWith("T")
        && returnType.EndsWith("**")
        && r.EndsWith("void**") ) return "";
    else if( returnType.BeginsWith("T")
             && returnType.EndsWith("*&")
             && r.EndsWith("void**") ) return "";
    else if( returnType.EndsWith("string")
             && r.EndsWith("char*") ) return "";
    else if( returnType.EndsWith("string&")
             && r.EndsWith("char*") ) return "";
    else if( returnType.EndsWith("vector<double>")
             && r.EndsWith("double*") ) return "";
    else if( returnType.EndsWith("vector<double>&")
             && r.EndsWith("double*") ) return "";
    else if( r.EndsWith("int*") ||
             r.EndsWith("long*") ||
             r.EndsWith("long int*") ||
             r.EndsWith("float*") ||
             r.EndsWith("double*") ||
             r.EndsWith("char**") ) {
      return "addr_t";
    }
    else return r;
  } catch( TString msg ) {
    throw msg + "\n  at convReturnType";
  }
}

/**
 * superClassがclassNameの親クラスか調べる。
 */
bool isSuperClass( const TString className,
                   const TString superClass ) {
  TClass *cls = TClass::GetClass( className );
  TIter nextBase( cls->GetListOfBases() );
  TObject *base;
  bool flag = false;
  while( (base = nextBase()) ) {
    TString baseName = base->GetName();
    if( superClass == baseName ) {
      flag = true;
      break;
    }
    else if( isSuperClass( baseName, superClass ) ) {
      flag = true;
      break;
    }
  }
  return flag;
}

TString implementsSupers( const TString className,
                          THashTable *imps,
                          THashTable *excludeTypes )
{
  TClass *cls = TClass::GetClass( className );
  TIter nextBase( cls->GetListOfBases() );
  TObject *base;
  TString supers = "Pointer";
  while( (base = nextBase()) ) {
    TString baseName = base->GetName();
    if( !has(imps, baseName) && !has(excludeTypes, baseName) ) {
      throw "Unsupported class for java interface: " + baseName;
    }
    else if( has(imps, baseName) &&
             !has(excludeTypes, baseName) ) {
      supers += ", " + baseName;
    }
  }
  return supers;
}

TString getPrototype( TMethod *method,
                      const TString &className ) {
  return TString(method->GetReturnTypeName()) + " "
    + className + "::"
    + method->GetName()
    + "(" + getMethodArgTypes(method) + ")";
}

/**
 * 再帰的に親クラスまでのメソッドを取得する。
 */
void _getMethods( const TString &className,
                  TMap *map ) {
  TClass *cls = TClass::GetClass( className );
  for( TIter next(cls->GetListOfAllPublicMethods());
       TMethod* m = (TMethod*)next(); ) {
    TString proto = getPrototype( m, "_" );
    if( !map->Contains( proto ) ) {
      map->Add( new TObjString( proto ), m );
    }
  }
  for( TIter next(cls->GetListOfBases());
       TObject* o = (TObject*)next(); ) {
    _getMethods( o->GetName(), map );
  }
}

TMap *getMethods( const TString &className ) {
  TMap *map = new TMap();
  _getMethods( className, map );
  //TClass *cls = TClass::GetClass( className );
  //for( TIter next(cls->GetListOfAllPublicMethods());
  //     TMethod* m = (TMethod*)next(); ) {
  //  TString proto = getPrototype( m, "_" );
  //  map->Add( new TObjString( proto ), m );
  //}
  return map;
}

/**
 * Cの関数の引数をROOTの関数の引数としてわたせるようにキャストとする。
 * @param org ROOT側の引数の型
 * @param now C側の引数の型
 */
TString castArg( TString corg, TString cnow, TString var ) {
  TString org = corg;
  TString now = cnow;
  org.ReplaceAll("const ", "");
  now.ReplaceAll("const ", "");
  TString torg = stripTypeName( org );
  // ROOTの型
  if( org.BeginsWith("T") &&
      org.EndsWith("***") &&
      now == "addr_t" )
    return TString::Format("reinterpret_cast<%s***>(%s)",
                           torg.Data(),
                           var.Data());
  // ROOTの型
  else if( org.BeginsWith("T") &&
           org.EndsWith("**") &&
           now == "addr_t" )
    return TString::Format("reinterpret_cast<%s**>(%s)",
                           torg.Data(),
                           var.Data());
  // ROOTの型
  else if( org.BeginsWith("T") &&
           org.EndsWith("*") &&
           now == "addr_t" )
    return TString::Format("reinterpret_cast<%s*>(%s)",
                           torg.Data(),
                           var.Data());
  // ROOTの型
  else if( org.BeginsWith("T") &&
           org.EndsWith("&") &&
           now == "addr_t" )
    return TString::Format("*reinterpret_cast<%s*>(%s)",
                           torg.Data(),
                           var.Data());
  else if( org.EndsWith("_t&") &&
           now == "addr_t" )
    return TString::Format("*reinterpret_cast<%s*>(%s)", torg.Data(), var.Data());
  else if( org.EndsWith("_t*") &&
           now == "addr_t" )
    return TString::Format("*reinterpret_cast<%s*>(%s)",
                           torg.Data(),
                           var.Data());
  else if( org.EndsWith("*&") &&
           now == "addr_t" )
    return TString::Format("*%s", var.Data());
  else if( org.EndsWith("void*") &&
           now == "addr_t" )
    return TString::Format("reinterpret_cast<%s*>(%s)",
                           torg.Data(),
                           var.Data());
  else if( org.EndsWith("*&") )
    return TString::Format("*%s", var.Data());
  else if( org.EndsWith("UInt_t*") )
    return TString::Format("(%s)%s", corg.Data(), var.Data());
  else if( org.EndsWith("UChar_t*") )
    return TString::Format("(%s)%s", corg.Data(), var.Data());
  else if( org.EndsWith("Long64_t*") )
    return TString::Format("(%s)%s", corg.Data(), var.Data());
  else return var;
  //if( org.EndsWith("&") && now == "void*") return "("+org+")*";
  //else if( org.EndsWith("&") && now.EndsWith("*") ) return "("+org+")*";
  //else if( org != "void*" && now == "void*" ) return "("+org+")";
  //else return "("+org+")";
}

/**
 * @param org ROOTの型
 * @param jtype JAVAの型
 * @param name 変数名
 */
TString castJNAArg( TString org, TString jtype, TString name )
{
  org.ReplaceAll("const ", "");
  TString torg = stripTypeName( org );
  if( torg.BeginsWith("T") &&
      (org.EndsWith("**") || org.EndsWith("*&")) ) {
    return name + ".getReference().getNativeAddress()";
  }
  else if( torg.BeginsWith("TString") &&
           (org.EndsWith("*") || org.EndsWith("&")) ) {
    return name;
  }
  else if( torg.BeginsWith("T") &&
           (org.EndsWith("*") || org.EndsWith("&")) ) {
    return name + ".getNativeAddress()";
  }
  else if( jtype == "Pointer" ) {
    return name + ".getNativeAddress()";
  }
  else return name;
}

/**
 * 戻り値をポインタに変換する必要がある場合には、「&」を返す。
 * @param ROOT側の型 
 * @param now C側の型
 */
TString castForReturn( TString &corg,
                       TString &cnow,
                       TString &value ) {
  TString org = corg;
  TString now = cnow;
  org.ReplaceAll("const ", "");
  now.ReplaceAll("const ", "");
  TString torg = stripTypeName( org );
  TString tnow = stripTypeName( now );
  // ROOTのクラスの参照を返す場合
  if( org.BeginsWith("T") &&
      org.EndsWith("&") &&
      now == "addr_t" )
    return TString::Format("reinterpret_cast<addr_t>(&(%s))",
                           value.Data());
  // ROOTのクラスのポインタの参照を返す場合
  else if( org.BeginsWith("T") &&
           org.EndsWith("*&") &&
           now == "addr_t" )
    return TString::Format("reinterpret_cast<addr_t>(&%s)",
                           value.Data());
  // ROOTのクラスの実態を返す場合は、コピーのポインタを作成する。
  else if( org.BeginsWith("T") &&
           !org.EndsWith("*") &&
           !org.EndsWith("&") &&
           now == "addr_t" )
    return TString::Format("reinterpret_cast<addr_t>"
                           "(new %s(%s))",
                           org.Data(),
                           value.Data());
  //else if( org.BeginsWith("T") && org.EndsWith("*") && now.EndsWith("void*") ) return "("+tnow+"*)";
  else if( org.EndsWith("&") &&
           now == "addr_t" ) 
    return TString::Format("reinterpret_cast<addr_t>(&%s)",
                           value.Data());
  else if( org.EndsWith("*") &&
           now == "addr_t" )
    return TString::Format("reinterpret_cast<addr_t>(%s)",
                           value.Data());
  else return value;
}

/**
 * 同じ名前のメソッドの数を取得する。
 */
int getNextCount( TMap *map, const TString &methodName ) {
  TInt *v = (TInt*)map->GetValue( methodName );
  if( v ) {
    v->x += 1;
    return v->x;
  }
  else {
    TInt *v = new TInt(1);
    map->Add( new TObjString(methodName), v );
    return v->x;
  }  
}

int getCount( TMap *map, const TString &methodName ) {
  TInt *v = (TInt*)map->GetValue( methodName );
  if( v ) return v->x;
  else 0;
}

/**
 * メソッドの呼び出しリテラル生成。
 * operatorとそれ以外を区別する。
 */
TString callCppMethod( const TString method ) {
  TString m = method;
  if( m.Contains("operator()") ) m = "";
  else if( m.Contains("operator") ) {
    m.ReplaceAll("operator", "");
    m = " " + m + " ";
  }
  else m = "." + method;
  return m;
}

bool matchReg( const THashTable *map, const TString &str ) {
  bool flag = false;
  for( TIter next(map); TObjString *s = (TObjString*)next(); ) {
    if( s->GetString() != ""
        && str.Contains( TRegexp(s->GetString()) ) ) {
      flag = true;
      break;
    }
  }
  return flag;
}

TString makeJFileHeader()
{
  TString src = "";
  src += "package net.blackruffy.root;\n";
  src += "\n";
  return src;
}

TString makeJClassHeader(THashTable *imps,
                         THashTable *excludeTypes,
                         TString className,
                         TString jclassImp)
{
  try {
    TString src = "";
    src += "public interface " + className + " ";
  
    TString sups = implementsSupers( className,
                                     imps,
                                     excludeTypes );
    if( sups != "" ) src += "extends " + sups;
    src += " {\n";
    src += "  public static " + className;
    src += " to" + className + "(long self) {\n";
    src += "    return new " + jclassImp + "(self);\n";
    src += "  }\n";
    src += "  public void destroy();\n";
    return src;
  }
  catch( TString ex ) {
    throw ex + "\n  at makeJClassHeader"; 
  }
}

TString impName( TString className )
{
  return "J" + className + "Imp";
}

TString makeJClassImpHeader(TString className, TString superName)
{
  TString src = "";
  src += "class " + className + " implements " + superName + " {\n";
  src += "  private long self = 0;\n";
  src += "  public " + className + "( long self ) {\n";
  src += "    this.self = self;\n";
  src += "  }\n";
  src += "  public long getNativeAddress() {\n";
  src += "    return this.self;\n";
  src += "  }\n";
  src += "  public void destroy() {\n";
  src += "    " + superName + "Native." + superName + "_destroy(this.self);\n";
  src += "  }\n";
  return src;
}

TString makeJClassNativeHeader(TString className)
{
  TString src = "";
  src += "class " + className + "Native {\n";
  src += "  static {\n";
  src += "    System.loadLibrary(\"JRoot\");\n";
  src += "  }\n";
  src += "  public static native void " + className + "_destroy(long self);\n";
  return src;
}

////////////////////////////////////////////////////////////
// メイン関数
////////////////////////////////////////////////////////////

int main() {
  try {

    // 実装するクラス
    THashTable *impClasses = new THashTable();
    add( impClasses, "TROOT");
    add( impClasses, "TApplication");
    add( impClasses, "TCanvas");
    add( impClasses, "TCanvasImp");
    add( impClasses, "TFrame");
    add( impClasses, "TPad");
    add( impClasses, "TLegend");
    add( impClasses, "TLegendEntry");
    add( impClasses, "TPave");
    add( impClasses, "TBox");
    add( impClasses, "TGraph");
    add( impClasses, "TGraphErrors");
    add( impClasses, "TMultiGraph");
    add( impClasses, "TH1F");
    add( impClasses, "TH1D");
    add( impClasses, "TH2F");
    add( impClasses, "TF1");
    add( impClasses, "TProfile");
    add( impClasses, "TFormula");
    add( impClasses, "TAxis");
    add( impClasses, "TTree");
    add( impClasses, "TObjArray");
    add( impClasses, "TList");
    add( impClasses, "THashList");
    add( impClasses, "TFriendElement");
    add( impClasses, "TBranch");
    add( impClasses, "TRefTable");
    add( impClasses, "TLeaf");
    add( impClasses, "TBasket");
    add( impClasses, "TKey");
    add( impClasses, "TDatime");
    add( impClasses, "TFile");
    add( impClasses, "TDirectory");
    add( impClasses, "TDirectoryFile");
    add( impClasses, "TArrayC");
    add( impClasses, "TArrayD");
    add( impClasses, "TArrayF");
    add( impClasses, "TUrl");
    add( impClasses, "TEventList");
    add( impClasses, "TEntryList");
    add( impClasses, "TIterator");
    add( impClasses, "TIter");
    add( impClasses, "TPrincipal");
    add( impClasses, "TMatrixD");
    add( impClasses, "TVectorD");
    add( impClasses, "TVectorF");
    add( impClasses, "TMethodCall");
    add( impClasses, "TFunction");
    add( impClasses, "TPoint");
    add( impClasses, "TContextMenu");
    add( impClasses, "TBrowser");
    add( impClasses, "TBrowserImp");
    add( impClasses, "TClonesArray");
    add( impClasses, "TCut");
    add( impClasses, "TSelector");
    add( impClasses, "TLatex");
    add( impClasses, "TText");
    add( impClasses, "TRandom");
    //add( impClasses, "TMath");
    add( impClasses, "TMathText");
    add( impClasses, "TColor");
    add( impClasses, "TNtuple");
    add( impClasses, "TNtupleD");
    add( impClasses, "TChain");
    add( impClasses, "TPostScript");
    add( impClasses, "TTime");
    add( impClasses, "TTimeStamp");
    add( impClasses, "TLorentzVector");
    add( impClasses, "TLorentzRotation");
    add( impClasses, "TRotation");
    add( impClasses, "TVector2");
    add( impClasses, "TVector3");
    add( impClasses, "TQuaternion");
    add( impClasses, "TObjString");
    add( impClasses, "TStyle");
  
    // 実装するインターフェース
    THashTable *impInterfaces = new THashTable();
    add( impInterfaces, "TH1");
    add( impInterfaces, "TH2");
    add( impInterfaces, "TObject");
    add( impInterfaces, "TNamed");
    add( impInterfaces, "TAttFill");
    add( impInterfaces, "TAttLine");
    add( impInterfaces, "TAttMarker");
    add( impInterfaces, "TAttText");
    add( impInterfaces, "TAttPad");
    add( impInterfaces, "TVirtualPad");
    add( impInterfaces, "TSeqCollection");
    add( impInterfaces, "TCollection");
    add( impInterfaces, "TArray");
    add( impInterfaces, "TAttAxis");
    add( impInterfaces, "TDictionary");
    add( impInterfaces, "TView");
    add( impInterfaces, "TVirtualPadPainter");
    add( impInterfaces, "TVirtualViewer3D");
    add( impInterfaces, "TVirtualPS");
    add( impInterfaces, "TSpline");

    THashTable *imps = concat( impClasses, impInterfaces );
    
    // Cのプリミティブ型に変換
    TMap *cTypes = new TMap();
    add( cTypes, "Int_t"                   , "int");
    add( cTypes, "const Int_t"             , "const int");
    add( cTypes, "Style_t"                 , "int");
    add( cTypes, "Version_t"               , "int");
    add( cTypes, "int"                     , "int");
    add( cTypes, "unsigned int"            , "int");
    add( cTypes, "Width_t"                 , "int");
    add( cTypes, "Font_t"                  , "int");
    add( cTypes, "Color_t"                 , "int");
    add( cTypes, "Short_t"                 , "int");
    add( cTypes, "Marker_t"                , "int");
    add( cTypes, "SCoord_t"                , "int");
    add( cTypes, "UShort_t"                , "int");
    add( cTypes, "UInt_t"                  , "int");
    add( cTypes, "int*"                    , "int*");
    add( cTypes, "Int_t*"                  , "int*");
    add( cTypes, "Int_t[4]"                , "int*");
    add( cTypes, "UInt_t*"                 , "int*");
    add( cTypes, "Int_t&"                  , "addr_t");
    add( cTypes, "UInt_t&"                 , "addr_t");
    add( cTypes, "Short_t&"                , "addr_t");
    add( cTypes, "const Int_t*"            , "const int*");
    add( cTypes, "long"                    , "long");
    add( cTypes, "Long_t"                  , "long");
    add( cTypes, "ULong_t"                 , "long");
    add( cTypes, "unsigned long"           , "long int");
    add( cTypes, "unsigned long int"       , "long int");
    add( cTypes, "unsigned long long"      , "long int");
    add( cTypes, "Long64_t"                , "long int");
    add( cTypes, "size_t"                  , "long int");
    add( cTypes, "ULong64_t"               , "long int");
    add( cTypes, "const size_t"            , "const long int");
    add( cTypes, "Long64_t*"               , "long int*");
    add( cTypes, "Long_t*"                 , "long int*");
    add( cTypes, "Long_t&"                 , "addr_t");
    add( cTypes, "time_t"                  , "long int");
    add( cTypes, "float"                   , "float");
    add( cTypes, "Float_t"                 , "float");
    add( cTypes, "Size_t"                  , "float");
    add( cTypes, "float*"                  , "float*");
    add( cTypes, "Float_t[3]"              , "float*");
    add( cTypes, "Float_t*"                , "float*");
    add( cTypes, "const float*"            , "const float*");
    add( cTypes, "const Float_t*"          , "const float*");
    add( cTypes, "float&"                  , "addr_t");
    add( cTypes, "Float_t&"                , "addr_t");
    add( cTypes, "const float&"            , "const addr_t");
    add( cTypes, "double"                  , "double");
    add( cTypes, "Double_t"                , "double");
    add( cTypes, "Stat_t"                  , "double");
    add( cTypes, "LongDouble_t"            , "double");
    add( cTypes, "Double_t*"               , "double*");
    add( cTypes, "Double_t[3]"             , "double*");
    add( cTypes, "const Double_t[4]"       , "double*");
    add( cTypes, "double*"                 , "double*");
    add( cTypes, "Double_t&"               , "addr_t");
    add( cTypes, "double&"                 , "addr_t");
    add( cTypes, "vector<double>"          , "double*");
    add( cTypes, "const Double_t*"         , "const double*");
    add( cTypes, "const double*"           , "const double*");
    add( cTypes, "const vector<double>&"   , "const double*");
    add( cTypes, "const double&"           , "const addr_t");
    add( cTypes, "Char_t"                  , "char");
    add( cTypes, "UChar_t"                 , "char");
    add( cTypes, "UChar_t*"                , "char*");
    add( cTypes, "char"                    , "char");
    add( cTypes, "Option_t"                , "const char");
    add( cTypes, "const char"              , "const char");
    add( cTypes, "char*"                   , "char*");
    add( cTypes, "Char_t*"                 , "char*");
    add( cTypes, "Char_t&"                 , "char*");
    add( cTypes, "const Char_t*"           , "const char*");
    add( cTypes, "const char*"             , "const char*");
    add( cTypes, "Option_t*"               , "const char*");
    add( cTypes, "const Option_t*"         , "const char*");
    add( cTypes, "Text_t"                  , "char*");
    add( cTypes, "TString"                 , "const char*");
    add( cTypes, "TString&"                , "const char*");
    add( cTypes, "const TString&"          , "const char*");
    add( cTypes, "const string&"           , "const char*");
    add( cTypes, "string"                  , "char*");
    add( cTypes, "char**"                  , "char**");
    add( cTypes, "char*&"                  , "char**");
    add( cTypes, "const char**"            , "const char**");
    add( cTypes, "const TString*"          , "const char**");
    add( cTypes, "bool"                    , "int");
    add( cTypes, "Bool_t"                  , "int");
    add( cTypes, "const Bool_t"            , "const int");
    add( cTypes, "const bool"              , "const int");
    add( cTypes, "void"                    , "void");
    add( cTypes, "void*"                   , "addr_t");
    add( cTypes, "const void*"             , "const addr_t");

    // C -> Java type
    TMap *javaTypes = new TMap();
    add(javaTypes, "addr_t",             "long");
    add(javaTypes, "const addr_t",       "long");
    add(javaTypes, "int",                "int");
    add(javaTypes, "int*",               "int[]");
    add(javaTypes, "const int",          "int");
    add(javaTypes, "const int*",         "int[]");
    add(javaTypes, "long",               "int");
    add(javaTypes, "long*",              "int[]");
    add(javaTypes, "const long",         "int");
    add(javaTypes, "const long*",        "int[]");
    add(javaTypes, "long int",           "long");
    add(javaTypes, "long int*",          "long[]");
    add(javaTypes, "const long int",     "long");
    add(javaTypes, "const long int*",    "long[]");
    add(javaTypes, "float",              "float");
    add(javaTypes, "float*",             "float[]");
    add(javaTypes, "const float",        "float");
    add(javaTypes, "const float*",       "float[]");
    add(javaTypes, "double",             "double");
    add(javaTypes, "double*",            "double[]");
    add(javaTypes, "const double",       "double");
    add(javaTypes, "const double*",      "double[]");
    add(javaTypes, "char",               "byte");
    add(javaTypes, "const char",         "byte");
    add(javaTypes, "char*",              "String");
    add(javaTypes, "const char*",        "String");
    add(javaTypes, "char**",             "String[]");
    add(javaTypes, "const char**",       "String[]");
    add(javaTypes, "void",               "void");

    TMap *jniTypes = new TMap();
    add(jniTypes, "void",        "void");
    add(jniTypes, "int",         "jint");
    add(jniTypes, "int[]",       "jintArray");
    add(jniTypes, "long",        "jlong");
    add(jniTypes, "long[]",      "jlongArray");
    add(jniTypes, "float",       "jfloat");
    add(jniTypes, "float[]",     "jfloatArray");
    add(jniTypes, "double",      "jdouble");
    add(jniTypes, "double[]",    "jdoubleArray");
    add(jniTypes, "String",      "jstring");
    add(jniTypes, "String[]",    "jobjectArray");
    add(jniTypes, "byte",        "jbyte");
    add(jniTypes, "byte[]",      "jbyteArray");

    THashTable *excludeJavaTypes = new THashTable();
    add( excludeJavaTypes, "void***" );

    // 実装しない型・クラス
    THashTable *excludeTypes = new THashTable();
    add( excludeTypes, "TSignalHandler" );
    add( excludeTypes, "TApplication::EExitOnException" );
    add( excludeTypes, "TApplicationImp" );
    add( excludeTypes, "TClass" );
    add( excludeTypes, "TQObject" );
    add( excludeTypes, "TBuffer" );
    add( excludeTypes, "TArchiveFile" );
    add( excludeTypes, "TFileCacheRead" );
    add( excludeTypes, "TFileCacheWrite" );
    add( excludeTypes, "TProcessID" );
    add( excludeTypes, "TFileOpenHandle" );
    add( excludeTypes, "TFile::EFileType" );
    add( excludeTypes, "TFile::EAsyncOpenStatus" );
    add( excludeTypes, "TUUID" );
    add( excludeTypes, "TStreamerInfo" );
    add( excludeTypes, "TTree::TClusterIterator" );
    add( excludeTypes, "TVirtualTreePlayer" );
    add( excludeTypes, "TVirtualPerfStats" );
    add( excludeTypes, "TTreeFormula" );
    add( excludeTypes, "TVirtualIndex" );
    add( excludeTypes, "TVectorT<double>" );
    add( excludeTypes, "TVectorT<float>" );
    add( excludeTypes, "TH1::EBinErrorOpt" );
    add( excludeTypes, "TVirtualHistPainter" );
    add( excludeTypes, "TMatrixT<double>" );
    add( excludeTypes, "TMatrixTBase<double>" );
    add( excludeTypes, "TMatrixTRow_const<double>" );
    add( excludeTypes, "TMatrixTRow<double>" );
    add( excludeTypes, "TObjLink" );
    add( excludeTypes, "TMatrixDSym" );
    add( excludeTypes, "TMatrixFBase" );
    add( excludeTypes, "ROOT::Fit::FitResult" );
    add( excludeTypes, "ROOT::Fit::FitResult::IModelFunction" );
    add( excludeTypes, "ROOT::Fit::BinData" );
    add( excludeTypes, "TFitResultPtr" );
    add( excludeTypes, "TDictionary::DeclId_t" );
    add( excludeTypes, "TDictAttributeMap" );
    add( excludeTypes, "CallFunc_t" );
    add( excludeTypes, "TMethodCall::EReturnType" );
    add( excludeTypes, "Rectangle_t" );
    add( excludeTypes, "TAttBBox2D" );
    add( excludeTypes, "TGMainFrame" );
    add( excludeTypes, "TContextMenuImp" );
    add( excludeTypes, "TClassMenuItem" );
    add( excludeTypes, "TWBox" );
    add( excludeTypes, "TWbox" );
    add( excludeTypes, "TMemberInspector" );
    add( excludeTypes, "ostream" );
    add( excludeTypes, "ofstream" );
    add( excludeTypes, "Bool_t(*)(const TGraph*,Int_t,Int_t)" );
    add( excludeTypes, "TMethod" );
    add( excludeTypes, "TTimer" );
    add( excludeTypes, "pair<TString,Double_t>" );
    add( excludeTypes, "Foption_t" );
    add( excludeTypes, "TVirtualFFT" );
    add( excludeTypes, "va_list" );
    add( excludeTypes, "TFile::ERelativeTo" );
    add( excludeTypes, "TFileMergeInfo" );
    add( excludeTypes, "TPRegexp" );
    add( excludeTypes, "TSelector::EAbort" );
    add( excludeTypes, "istream" );
    add( excludeTypes, "TToggle" );
    add( excludeTypes, "TMethodArg" );
    add( excludeTypes, "TMatrixTSym<double>" );
    add( excludeTypes, "MethodInfo_t" );
    add( excludeTypes, "type_info" );
    add( excludeTypes, "wchar_t" );
    add( excludeTypes, "ECursor" );
    add( excludeTypes, "EEventType" );
    add( excludeTypes, "shared_ptr<ROOT::Math::Minimizer>" );
    add( excludeTypes, "ROOT::EFunctionMatchMode" );
    add( excludeTypes, "Double_t(*)(Double_t*,Double_t*)" );
    add( excludeTypes, "Double_t(*)(const Double_t*,const Double_t*)" );
    add( excludeTypes, "ROOT::Math::ParamFunctor" );
    add( excludeTypes, "TBranchRef");
    add( excludeTypes, "TSQLResult" );
    add( excludeTypes, "TSQLRow");
    add( excludeTypes, "TFitResultPtr");
    add( excludeTypes, "TFitResult" );
    add( excludeTypes, "TVirtualPadPainter::ETextMode" );
    add( excludeTypes, "TVirtualPadPainter::EBoxMode" );
    add( excludeTypes, "TBuffer3D" );
    add( excludeTypes, "unsigned char" );
    add( excludeTypes, "tm_t" );
    add( excludeTypes, "timespec_t" );
    add( excludeTypes, "TPoints" );
    add( excludeTypes, "TLorentzRotation::TLorentzRotationRow" );
    add( excludeTypes, "TRotation::TRotationRow" );
    add( excludeTypes, "TPluginManager" );
    add( excludeTypes, "TFunctionTemplate" );
    add( excludeTypes, "TDataType" );
    add( excludeTypes, "TFolder" );
    add( excludeTypes, "TGlobal" );
    add( excludeTypes, "TProcessUUID" );
    add( excludeTypes, "TClassGenerator" );
    add( excludeTypes, "TInterpreter" );
    add( excludeTypes, "TStyle::EPaperSize" );
    add( excludeTypes, "void*&" );
    add( excludeTypes, "const char**&" );
    add( excludeTypes, "void(*)()" );
    add( excludeTypes, "VoidFuncPtr_t" );

    // 変換するメソッド名
    TMap *operators = new TMap();
    add( operators, "New",               "_new_" );
    add( operators, "Import",            "_import_" );
    add( operators, "Notify",            "_notify_" );
    //add( operators, "operator[]",        "_at_" );
    add( operators, "operator()",        "_get_" );
    add( operators, "operator=",         "_set_" );
    add( operators, "operator+=",        "_addEq_" );
    add( operators, "operator-=",        "_subEq_" );
    add( operators, "operator*=",        "_multEq_" );
    add( operators, "operator/=",        "_divEq_" );
    add( operators, "operator==",        "_eq_" );
    add( operators, "operator!=",        "_notEq_" );
    add( operators, "operator+",         "_add_" );
    add( operators, "operator-",         "_sub_" );
    add( operators, "operator*",         "_mult_" );
    add( operators, "operator/",         "_div_" );
    add( operators, "operator>",         "_gt_" );
    add( operators, "operator<",         "_lt_" );
    add( operators, "operator>=",        "_gtEq_" );
    add( operators, "operator<=",        "_ltEq_" );
    add( operators, "operator->",        "_arrow_" );
    add( operators, "operator++",        "_increment_" );

    THashTable *excludeOperators = new THashTable();
    add( excludeOperators, "operator int" );
    add( excludeOperators, "operator long" );
    add( excludeOperators, "operator unsigned long" );
    add( excludeOperators, "operator unsigned long int" );
    add( excludeOperators, "operator unsigned long long" );
    add( excludeOperators, "operator long int" );
    add( excludeOperators, "operator long long" );
    add( excludeOperators, "operator double" );
    add( excludeOperators, "operator const char *" );
    add( excludeOperators, "operator new" );
    add( excludeOperators, "operator new[]" );
    add( excludeOperators, "operator delete" );
    add( excludeOperators, "operator delete[]" );
    add( excludeOperators, "operator[]");

    THashTable *excludeMethods = new THashTable();
    add( excludeMethods, "void TFormula_addVariables_1\\(addr_t self, const char\\*\\* vars, const int size\\)" );
    add( excludeMethods, "_mult__[0-9]+\\(addr_t self\\)" );
    add( excludeMethods, "__sub__[0-9]+\\(addr_t self\\)" );
    add( excludeMethods, "_increment__[0-9]+\\(addr_t self\\)" );
    add( excludeMethods, "_set_" );
    add( excludeMethods, "addr_t TPostScript_newTPostScript_1\\(const addr_t arg1\\)" );
    add( excludeMethods, "const char\\* TObjString_string_1\\(addr_t self)");

    // Cのラッパー関数の追加メソッドの宣言
    TMap *additionalHeaderMethods = new TMap();
    add( additionalHeaderMethods, "JRoot",
         "  addr_t JRoot_gROOT();\n"
         "  addr_t JRoot_gApplication();\n"
         "  addr_t JRoot_gStyle();\n"
         "  addr_t JRoot_gPad();\n"
         "  addr_t JRoot_gRandom();\n");
    add( additionalHeaderMethods, "TTree",
         "  int TTree_setBranchAddress_100(addr_t self, const char* bname, const char* className, addr_t add);\n"
         "  void TTree_debug_printAllEvent(addr_t self, addr_t vec);\n");
    add( additionalHeaderMethods, "TLorentzVector",
         "  void TLorentzVector_debug_getX(addr_t self, addr_t rval);\n" );

    // Cのラッパー関数の追加メソッドの実装
    TMap *additionalCppMethods = new TMap();
    add( additionalCppMethods, "JRoot",
         "addr_t JRoot_gROOT() {\n"
         "  return reinterpret_cast<addr_t>(gROOT);\n"
         "}\n"
         "addr_t JRoot_gApplication() {\n"
         "  return reinterpret_cast<addr_t>(gApplication);\n"
         "}\n"
         "addr_t JRoot_gStyle() {\n"
         "  return reinterpret_cast<addr_t>(gStyle);\n"
         "}\n"
         "addr_t JRoot_gPad() {\n"
         "  return reinterpret_cast<addr_t>(gPad);\n"
         "}\n"
         "addr_t JRoot_gRandom() {\n"
         "  return reinterpret_cast<addr_t>(gRandom);\n"
         "}\n");
    add( additionalCppMethods, "TTree",
         "int TTree_setBranchAddress_100(addr_t self, const char* bname, const char* className, addr_t add) {\n"
         "  TTree* tree = reinterpret_cast<TTree*>(self);\n"
         "  struct wrap { TObject* obj; };\n"
         "  wrap* p = (wrap*)malloc(sizeof(wrap));\n"
         "  p->obj = reinterpret_cast<TObject*>(add);\n"
         "  return tree->SetBranchAddress(bname, &p->obj);\n"
         "}\n"
         "void TTree_debug_printAllEvent(addr_t self, addr_t vec) {\n"
         "  long int nev = TTree_getEntries_1(self);\n"
         "  for( int ev=0; ev<nev; ev++ ) {\n"
         "    TTree_getEntry_2(self, ev);\n"
         "    std::cout << ev << \" \" << TLorentzVector_x_1(vec) << std::endl;\n"
         "  }\n"
         "}\n");
    add( additionalCppMethods, "TLorentzVector",
         "void TLorentzVector_debug_getX(addr_t self, addr_t rval) {\n"
         "  double* v = reinterpret_cast<double*>(rval);\n"
         "  *v = TLorentzVector_x_1(self);\n"
         "}\n");

    TMap* additionalJNIMethods = new TMap();
    add( additionalJNIMethods, "JRootNative",
         "JNIEXPORT jlong JNICALL Java_net_blackruffy_root_JRootNative_JRoot_1gROOT(JNIEnv *env, jclass thisj) {\n"
         "  return JRoot_gROOT();\n"
         "}\n"
         "JNIEXPORT jlong JNICALL Java_net_blackruffy_root_JRootNative_JRoot_1gApplication(JNIEnv *env, jclass thisj) {\n"
         "  return JRoot_gApplication();\n"
         "}\n"
         "JNIEXPORT jlong JNICALL Java_net_blackruffy_root_JRootNative_JRoot_1gStyle(JNIEnv *env, jclass thisj) {\n"
         "  return JRoot_gStyle();\n"
         "}\n"
         "JNIEXPORT jlong JNICALL Java_net_blackruffy_root_JRootNative_JRoot_1gPad(JNIEnv *env, jclass thisj) {\n"
         "  return JRoot_gPad();\n"
         "}\n"
         "JNIEXPORT jlong JNICALL Java_net_blackruffy_root_JRootNative_JRoot_1gRandom(JNIEnv *env, jclass thisj) {\n"
         "  return JRoot_gRandom();\n"
         "}\n");
    add( additionalJNIMethods, "TTree",
         "JNIEXPORT jint JNICALL Java_net_blackruffy_root_TTreeNative_TTree_1setBranchAddress_1100(JNIEnv *env, jclass thisj, jlong self, jstring bname, jstring className, jlong add) {\n"
         "  const char* jni_bname = env->GetStringUTFChars(bname, NULL);\n"
         "  const char* jni_className = env->GetStringUTFChars(className, NULL);\n"
         "  jint __rval__ = TTree_setBranchAddress_100(self, jni_bname, jni_className, add);\n"
         "  env->ReleaseStringUTFChars(bname, jni_bname);\n"
         "  env->ReleaseStringUTFChars(className, jni_className);\n"
         "  return __rval__;\n"
         "}\n");
    
    // JavaのJNAの追加メソッド
    TMap* additionalJNAMethods = new TMap();
    add( additionalJNAMethods, "JRootNative",
         "  public static native long JRoot_gROOT();\n"
         "  public static native long JRoot_gApplication();\n"
         "  public static native long JRoot_gStyle();\n"
         "  public static native long JRoot_gPad();\n"
         "  public static native long JRoot_gRandom();\n");
    add( additionalJNAMethods, "TTree",
         "  public static native int TTree_setBranchAddress_100(long self, String bname, String className, long add);\n"
         "  public static native void TTree_debug_printAllEvent(long self, long vec);\n");
    add( additionalJNAMethods, "TLorentzVector",
      "  public static native void TLorentzVector_debug_getX(long self, long rval);\n" );

    // JAVAの公開インターフェースの追加メソッド
    TMap* additionalJPubMethods = new TMap();
    add( additionalJPubMethods, "JRoot",
         "  public static TROOT gROOT() {\n"
         "    return TROOT.toTROOT(JRootNative.JRoot_gROOT());\n"
         "  }\n"
         "  public static TApplication gApplication() {\n"
         "    return TApplication.toTApplication(JRootNative.JRoot_gApplication());\n"
         "  }\n"
         "  public static TStyle gStyle() {\n"
         "    return TStyle.toTStyle(JRootNative.JRoot_gStyle());\n"
         "  }\n"
         "  public static TVirtualPad gPad() {\n"
         "    return TVirtualPad.toTVirtualPad(JRootNative.JRoot_gPad());\n"
         "  }\n"
         "  public static TRandom gRandom() {\n"
         "    return TRandom.toTRandom(JRootNative.JRoot_gRandom());\n"
         "  }\n");
    add( additionalJPubMethods, "TTree",
         "  default void debug_printAllEvent(TTree self, TLorentzVector vec){\n"
         "    TTreeNative.TTree_debug_printAllEvent(self.getNativeAddress(), vec.getNativeAddress());\n"
         "  }\n");

    add( additionalJPubMethods, "TLorentzVector",
         "    static void debug_getX(long self, long rval) {\n"
         "      TLorentzVectorNative.TLorentzVector_debug_getX(self, rval);\n"
         "    }\n");
    
    // JAVAの実装クラスの追加メソッド
    TMap *additionalJavaMethods = new TMap();
    add( additionalJavaMethods,
         "TH1F",
         "    public void setTitle() {\n"
         "      setTitle(\"\");\n"
         "    }\n");
    add( additionalJavaMethods,
         "TH1",
         "    public void setTitle() {\n"
         "      setTitle(\"\");\n"
         "    }\n");
    add( additionalJavaMethods,
         "TSpline",
         "    public void saveAs(String filename) {\n"
         "      saveAs(filename, \"\");\n"
         "    }\n"
         "    public void saveAs() {\n"
         "      saveAs(\"\");\n"
         "    }\n");
    add( additionalJavaMethods,
         "TProfile",
         "    public void setTitle() {\n"
         "      setTitle(\"\");\n"
         "    }\n");
    add( additionalJavaMethods,
         "TH1D",
         "    public void setTitle() {\n"
         "      setTitle(\"\");\n"
         "    }\n");
    add( additionalJavaMethods,
         "TH2",
         "    public void fillN(int ntimes, double[] x, double[] w) {\n"
         "      fillN(ntimes, x, w, 1);\n"
         "    }\n"
         "    public int getBin(int binx) {\n"
         "      return getBin(binx, 0);\n"
         "    }\n"
         "    public void setTitle() {\n"
         "      setTitle(\"\");\n"
         "    }\n");
    add( additionalJavaMethods,
         "TH2F",
         "    public void fillN(int ntimes, double[] x, double[] w) {\n"
         "      fillN(ntimes, x, w, 1);\n"
         "    }\n"
         "    public int getBin(int binx) {\n"
         "      return getBin(binx, 0);\n"
         "    }\n"
         "    public void setTitle() {\n"
         "      setTitle(\"\");\n"
         "    }\n");
    add( additionalJavaMethods,
         "TTree",
         "    public void draw() {\n"
         "      draw(\"\");\n"
         "    }\n");
    add( additionalJavaMethods,
         "TChain",
         "    public int loadBaskets() {\n"
         "      return loadBaskets(2000000000);\n"
         "    }\n"
         "    public void draw() {\n"
         "      draw(\"\");\n"
         "    }\n");
    add( additionalJavaMethods,
         "TNtuple",
         "    public void draw() {\n"
         "      draw(\"\");\n"
         "    }\n");
    add( additionalJavaMethods,
         "TNtupleD",
         "    public void draw() {\n"
         "      draw(\"\");\n"
         "    }\n");
    add( additionalJavaMethods,
         "TObjString",
         "    public void print() {\n"
         "      print(\"\");\n"
         "    }\n");

    TMap* overrideJavaMethods = new TMap();
    add( overrideJavaMethods,
         "TTree::SetBranchAddress;1",
         "  public int setBranchAddress(String bname, Pointer add) {\n"
         "    if(add instanceof TObject) {\n"
         "      return TTreeNative.TTree_setBranchAddress_100(self, bname, \"TObject\", add.getNativeAddress());\n"
         "    }\n"
         "    else {\n"
         "      return TTreeNative.TTree_setBranchAddress_1(self, bname, add.getNativeAddress());\n"
         "    }\n"
         "  }\n");
    add( overrideJavaMethods,
         "TChain::SetBranchAddress;1",
         "  public int setBranchAddress(String bname, Pointer add) {\n"
         "    if(add instanceof TObject) {\n"
         "      return TTreeNative.TTree_setBranchAddress_100(self, bname, \"TObject\", add.getNativeAddress());\n"
         "    }\n"
         "    else {\n"
         "      return TTreeNative.TTree_setBranchAddress_1(self, bname, add.getNativeAddress());\n"
         "    }\n"
         "  }\n");

    
  
    TString cpp_output_dir = "build/src/cpp";
    TString java_output_dir = "build/src/java";
  
    ofstream h_file(cpp_output_dir + "/croot.h");
    ofstream cpp_file(cpp_output_dir + "/croot.cpp");

    //ofstream jh_file(java_output_dir + "/JRoot.java");

    h_file << "#include <addr.h>" << endl;
      
    // ヘッダのインクルード
    for( TIter next( imps );
         TObjString *key = (TObjString*)next(); ) {
      TString className = key->GetString();
      if( className != "TIter" )
        h_file << "#include <" << className << ".h>" << endl;
    }

    h_file << "" << endl;
    h_file << "extern \"C\" {" << endl;
    h_file << get(additionalHeaderMethods, "JRoot");
  
    cpp_file << "#include <croot.h>" << endl;
    cpp_file << get(additionalCppMethods, "JRoot");

    {
      ofstream jnicpp_file(cpp_output_dir + "/net_blackruffy_root_JRootNative.cpp");
      jnicpp_file << "#include <net_blackruffy_root_JRootNative.h>\n";
      jnicpp_file << "#include <croot.h>\n";
      jnicpp_file << get(additionalJNIMethods, "JRootNative");
      jnicpp_file.close();
    }
    
    TString jroot = "";
    jroot += "package net.blackruffy.root;\n";
    jroot += "\n";
    jroot += "class JRootNative {\n";
    jroot += "  static {\n";
    jroot += "    System.loadLibrary(\"JRoot\");\n";
    jroot += "  }\n";
    jroot += get( additionalJNAMethods, "JRootNative" );
    jroot += "}\n";
    jroot += "public interface JRoot {\n";
    jroot += get( additionalJPubMethods, "JRoot" );
    
    
    // 実装クラスのループ
    for( TIter next( imps );
         TObjString *key = (TObjString*)next(); ) {
    
      TString className = key->GetString();
      TClass *cls = TClass::GetClass( className );
      if( !cls ) {
        cerr << "[ERROR] could not get class: " << className << endl;
        abort();
      }
      bool isInterface = has(impInterfaces, className);

      cout << className << endl;
      
      TMap *methodMap = getMethods( className );
      TMap *mcounts = new TMap();

      h_file << additionalHeaderMethod(className);
      cpp_file << additionalCppMethod(className);

      // JNI C++実装
      TString jnicpp = "";
      jnicpp += "#include <net_blackruffy_root_"+className+"Native.h>\n";
      jnicpp += "#include <croot.h>\n";
      jnicpp += "\n";
      jnicpp += "JNIEXPORT void JNICALL Java_net_blackruffy_root_"+className+"Native_TH1F_1destroy(JNIEnv *env, jclass thisj, jlong self) {\n";
      jnicpp += "  "+className+"_destroy(self);\n";
      jnicpp += "}\n";
      
      // 実装クラス
      TString jclassImp = impName( className );
      TString jimp = "";
      jimp += makeJClassImpHeader( jclassImp, className );
      
      // Javaファイル
      TString jfile = "";
      jfile += makeJFileHeader();

      // ラッパーインターフェース
      TString jwrapper = "";
      jwrapper += makeJClassHeader( imps,
                                    excludeTypes,
                                    className,
                                    jclassImp);
      
      // Nativeクラス
      TString jnative = "";
      jnative += makeJClassNativeHeader( className );

      // JRootクラスのキャストメソッド
      jroot += "  public static "
        + className + " " + className
        +"(Pointer obj) {\n";
      jroot += "    return " + className + ".to"
        + className + "(obj.getNativeAddress());\n";
      jroot += "  }\n";
      
      THashTable *appliedArgs = new THashTable();
      
      //___________________________________________
      // メソッドループ
      for( TIter nextM(methodMap);
           TObjString *mkey = (TObjString*)nextM(); ) {
        TMethod *method = (TMethod*)methodMap->GetValue( mkey->GetString() );
      
        TString defMethod = getPrototype( method, className );
        //if( className == "TLegend" ) cout << ">>>>>>> " << defMethod << endl;
        try {
          if( !isSuperClass( className, method->GetName() )
              && !hasInvalidType( excludeTypes, method ) ) {

            cpp_file << "// " << defMethod << endl;
            //if( className == "TNamed" ) cout << ">>>>>>> " << defMethod << endl;

            TString methodName = method->GetName();
            TString _methodName = convMethodName( operators,
                                                  excludeOperators,
                                                  methodName );

            // オリジナル(ROOT)の戻り値
            TString returnType = method->GetReturnTypeName();
            // Cラッパー関数の戻り値
            TString _returnType = convReturnType( cTypes,
                                                  imps,
                                                  excludeTypes,
                                                  returnType );

            bool isConstructor = methodName == className;
            bool isDestructor = methodName.BeginsWith("~");
            bool isCopyOp = methodName == "operator=";
            bool invalidInterfaceMethod
              = isInterface && isConstructor;
        
            bool invalidModifyMethod
              = methodName == "Modify"
              && getCount( mcounts, methodName ) > 1;
        
            // コンストラクタの場合
            if( isConstructor ) {
              _methodName = "new" + className;
              _returnType = "addr_t";
              //cout << defMethod << endl;
            }

            // JNIのJAVA側の関数の戻り値の型
            TString jreturnType = "void*";
            
            // Javaの公開インターフェースの戻り値の型
            // Javaの内部実装関数の戻り値の型
            TString wreturnType = "void";
            
            if( _returnType != "" && !isConstructor ) {
              jreturnType = convJNAType( javaTypes,
                                         returnType,
                                         _returnType );
              wreturnType = convJavaType( imps,
                                          javaTypes,
                                          cTypes,
                                          returnType );
              if( wreturnType == "int[]" ||
                  wreturnType == "long[]" ||
                  wreturnType == "float[]" ||
                  wreturnType == "double[]" ||
                  wreturnType == "String[]") {
                wreturnType = "Pointer";
              }
              //if( jreturnType == "Pointer" )
              //cout << ">>> return: " << _returnType << " " << jreturnType << " | " << defMethod << endl;
            }
            else if( isConstructor ) {
              jreturnType = "long";
            }


            if( isConstructor ) wreturnType = className;

            // JNI C++ 戻り値の型
            TString jnicppRetrunType = "";
            if( _returnType != "" ) {
              if( has(jniTypes, jreturnType) ) {
                jnicppRetrunType = get(jniTypes, jreturnType);
              }
              else {
                throw TString::Format("failed to get return type for %s JNI C++ at convJNIReturnType", jreturnType.Data());
              }
            }
            
            
            //______________________________
            // 正常な戻り値とメソッド名の場合は、
            // 引数の処理をする。
            if( _returnType != "" &&
                _methodName != "" &&
                !invalidModifyMethod &&
                !invalidInterfaceMethod ) {
              
              //if( className == "TH1F" ) cout << ">>>>>>> " << defMethod << endl;
              
              // 引数のC++やROOTの型をCの型に変換
              TList *xs = convArgTypes( cTypes,
                                        imps,
                                        excludeTypes,
                                        className,
                                        methodName,
                                        method->GetListOfMethodArgs() );

              //___________________________
              // 関数のオーバーロードのループ
              // デフォルト引数がある場合には、
              // xsは２つ以上の要素を持つ。
              for( TIter next1(xs);
                   TList *as = (TList*)next1(); ) {
                TString sas = ""; // 関数定義の引数(C Wrapper)
                TString jas = ""; // 関数定義の引数(Java JNA)
                TString was = ""; // 関数定義の引数(Java Wrapper)
                TString jniargs = ""; // 関数定義の引数(JNI C++)
                // CのラッパーからROOT関数に渡す変数
                TString rvar = ""; 
                // JAVAの実装からJNA関数に渡す変数
                TString jvar = "";
                // JNI内でCの関数に渡す変数
                JNIArg jnivar; 
                TString jdups = _methodName; // 重複メソッドチェック用
                TString hvar = ""; // JRootのコンストラクタ引数

                // コンスタラクタじゃない場合は、
                // 引数の最初にインスタンスのポインタを付ける。
                if( !isConstructor ) {
                  sas += "addr_t self";
                  jas += "long self";
                  jniargs += ", jlong self";
                  jnivar._3 += "self";
                }

                //__________________________
                // 引数のループ
                for( TIter next2(as); Arg *a = (Arg*)next2(); ) {

                  // Cラッパー関数定義の引数
                  if( sas != "" ) sas += ", ";
                  sas += a->type + " " + a->name;

                  // to java
                  if( has( excludeJavaTypes, a->type ) ) {
                    cerr << "[ERROR] invalid java type: "
                         << a->type
                         << " at "
                         << defMethod
                         << endl;
                  }

                  // JNAの型に変換
                  // JNI C++の型に変換
                  {
                    TString jtype = convJNAType( javaTypes,
                                                 a->orgType,
                                                 a->type );
                    jtype = convSpecialJNAType(className, methodName, a, jtype);
                    if( jas != "" ) jas += ", ";
                    jas += jtype + " " + a->name;

                    if( has(jniTypes, jtype) ) {
                      jniargs += ", " + get(jniTypes, jtype) + " " + a->name;
                    }
                    else {
                      throw TString::Format("failed to get jni arg type for %s", a->name.Data());
                    }

                    jnivar.append(convJNIArg(jniTypes, jtype, a->name));
                    
                    // for debug
                    //if( className == "TApplication" &&
                    //    methodName == "Browse" ) {
                    //  cout << methodName << " | " << jtype << " | " << a->orgType << " | " << a->type << " | " << a->name << " | " << defMethod << endl;
                    //}
                  }

                  // Javaの公開インターフェースの型に変換
                  // Javaの内部実装クラスの型でもある。
                  {
                    TString jtype = convJavaType( imps,
                                                  javaTypes,
                                                  cTypes,
                                                  a->orgType );
                    jtype = convSpecialJavaType(className, methodName, a, jtype);
                    if( was != "" ) was += ", ";
                    was += jtype + " " + a-> name;
                    
                    if( jdups != "" ) jdups += " ";
                    jdups += jtype;

                    // JNA関数に適用する変数を生成する。
                    if( jvar != "" ) jvar += ", ";
                    jvar += castJNAArg(a->orgType, jtype, a->name);
                  }

                  //-----------
                  // ROOTの関数に適用するために
                  // Cの型をROOTの型に変換する
                  // キャスト演算子を生成する。
                  {
                    TString castlit = castArg( a->orgType,
                                               a->type,
                                               a->name );
                    castlit = convSpecialCType( className,
                                                methodName,
                                                a,
                                                castlit );
                    if( rvar == "" ) rvar = castlit;
                    else rvar += ", " + castlit;
                  }

                  {
                    if( hvar != "" ) hvar += ", ";
                    hvar += a->name;
                  }
              
                } // 引数のループ
                //////////////////////////

                int mnum = getNextCount(mcounts, _methodName);
                TString smnum = TString::Format("%d", mnum);
                
                // Cの関数の宣言
                TString proto = _returnType + " "
                  + className + "_"
                  + _methodName + "_"
                  + smnum
                  + "(" + sas + ")";

                
                // 同じメソッドを定義していないこと。
                // 除外するメソッドに含まれてないこと。
                if( !has( appliedArgs, jdups ) &&
                    !matchReg( excludeMethods, proto) ) {
                  //if( className == "TFunction" ) cout << ">>>>>>> " << defMethod << endl;
                  
                  // JNIのJAVA側の関数の宣言
                  TString jproto = "  public static native " + jreturnType + " "
                    + className + "_"
                    + _methodName + "_"
                    + smnum
                    + "(" + jas + ");\n";

                  //-----
                  // Javaの公開インターフェース
                  TString wproto = "  ";
                  if( isConstructor ) wproto += "static ";
                  wproto += "public "
                    + wreturnType + " "
                    + _methodName
                    + "(" + was + ")";
                  if( isConstructor ) {
                    wproto += " {\n";
                    wproto += "    return new "
                      + jclassImp
                      + "(" + className
                      + "Native."
                      + className + "_"
                      + _methodName + "_"
                      + smnum
                      + "(" + jvar + ")" + ");\n";
                    wproto += "  }\n";
                  }
                  else wproto += ";\n";

                  //------
                  // JAVAの実装クラスのメソッド
                  TString orjm = className + "::"
                    + methodName + ";"
                    + smnum;
                  if( has( overrideJavaMethods, orjm ) ) {
                    jimp += get(overrideJavaMethods, orjm );
                  }
                  else if( !isConstructor ) {
                    jimp += "  public " + wreturnType + " " + _methodName + "(" + was + ") {\n";
                    if( jreturnType != "void" ) jimp += "    return ";
                    else jimp += "    ";

                    // 戻り値の型変換
                    if( jreturnType == "long" &&
                        wreturnType.BeginsWith("T") )
                      jimp += wreturnType + ".to" + wreturnType + "(";
                    else if( wreturnType == "Pointer" )
                      jimp += "Pointer.newPointer(";

                    // JNA関数の呼び出し
                    jimp += className + "Native." + className + "_" + _methodName + "_" + smnum;

                    // JNA関数に引数を代入
                    if( jvar == "" ) jimp += "(self)";
                    else jimp += "(self, "+jvar+")";
                    
                    if( jreturnType == "long" &&
                        wreturnType.BeginsWith("T") )
                      jimp += ")";
                    else if( wreturnType == "Pointer" )
                      jimp += ")";
                    
                    jimp += ";\n";
                    jimp += "  }\n";
                  }

                  //------
                  // JRootクラス
                  // インスタンス生成メソッド
                  // キャストメソッド
                  if( isConstructor ) {
                    jroot += "  public static "
                      + className + " new" + className
                      + "(" + was + ") {\n";
                    jroot += "    return " + className
                      + ".new" + className + "(" + hvar + ");\n";
                    jroot += "  }\n";
                  }

                  // プロトタイプ出力
                  h_file << "  " << proto << ";" << endl;

                  ////////////////////////////////
                  // Cのラッパー関数の実装
                  ////////////////////////////////
                  TString cmethod = "";
                  cmethod += proto + " {\n";
                  TString cppMethodName
                    = callCppMethod( methodName );
                  TString baseClassName = method->GetClass()->GetName();
                  //TString baseClassName = className;
                  TString callee = className;

                  // コンストラクタ
                  if( isConstructor )
                    cmethod += TString::Format
                      ("  return reinterpret_cast"
                       "<addr_t>(new %s(%s));\n",
                       methodName.Data(),
                       rvar.Data());
                  // デストラクタ
                  else if( isDestructor )
                    cmethod += TString::Format
                      ("  delete reinterpret_cast"
                       "<%s*>(self);\n",
                       callee.Data());
                  // 戻り値なし
                  else if( returnType == "void" )
                    cmethod += TString::Format
                      ("  (*(%s*)reinterpret_cast<%s*>"
                       "(self))%s(%s);\n",
                       baseClassName.Data(),
                       callee.Data(),
                       cppMethodName.Data(),
                       rvar.Data());
                  // 戻り値あり
                  else {
                    TString tmp = TString::Format
                      ("(*(%s*)reinterpret_cast<%s*>"
                       "(self))%s(%s)",
                       baseClassName.Data(),
                       callee.Data(),
                       cppMethodName.Data(),
                       rvar.Data());
                    cmethod += TString::Format
                      ("  return %s;\n",
                       castForReturn(returnType,
                                     _returnType,
                                     tmp).Data());
                  }
                  cmethod += "};\n";

                  ////////////////////////////////
                  // JNI C++の実装
                  ////////////////////////////////
                  {
                    TString src = "";
                    src += "JNIEXPORT "+jnicppRetrunType+" JNICALL Java_net_blackruffy_root_"+className+"Native_"+className+"_1"+_methodName+"_1"+smnum+"(JNIEnv *env, jclass thisj"+jniargs+") {\n";
                    src += "  " + jnivar._1;
                    src += "  ";
                    if( jnicppRetrunType != "void" )
                      src += _returnType+"  __rval__ = ";
                    src += className+"_"+_methodName+"_"+smnum+"("+jnivar._3+");\n";
                    JNIArg jretv;
                    if( jnicppRetrunType != "void" ) {
                      jretv.set(convJNIReturnValue(jnicppRetrunType, "__rval__"));
                      src += "  " + jretv._1;
                    }
                    src += "  " + jnivar._2;
                    src += "  " + jretv._2;
                    if( jnicppRetrunType != "void" ) src += "  return "+jretv._3+";\n";
                    src += "}\n";
                    jnicpp += src;
                  }
                  
                  //cout << cmethod << endl;
                  cpp_file << cmethod << endl;
                  
                  jwrapper += wproto;
                  jnative += jproto;
                
                
                  // 定義した関数の引数を記録する。
                  // 同じ定義がないかチェックするために使う。
                  add( appliedArgs, jdups );
                  
                } // 重複定義チェック
                
              } // 関数のオーバーロードのループ
            } // 戻り値チェック
          } // クラスチェック
        } catch( TString msg ) {
          throw msg + "\n  at " + defMethod;
        }
      } // メソッドループ

      if( has(additionalHeaderMethods, className) ) {
        h_file << get(additionalHeaderMethods, className);
      }

      if( has(additionalCppMethods, className) ) {
        cpp_file << get(additionalCppMethods, className);
      }

      if( has(additionalJNIMethods, className ) ) {
        jnicpp += get( additionalJNIMethods, className );
      }

      if( has(additionalJNAMethods, className ) ) {
        jnative += get( additionalJNAMethods, className );
      }

      if( has(additionalJPubMethods, className) ) {
        jwrapper += get(additionalJPubMethods, className);
      }
      
      if( has( additionalJavaMethods, className ) ) {
        jimp += get( additionalJavaMethods, className );
      }

      jwrapper += "}\n";
      jnative += "}\n";
      jimp += "}\n";
      jfile += jnative;
      jfile += jimp;
      jfile += jwrapper;
      
      
      // Javaのクラス生成
      ofstream jclass_file(java_output_dir + "/" + className + ".java");
      jclass_file << jfile << endl;
      jclass_file.close();

      ofstream jnicpp_file(cpp_output_dir + "/net_blackruffy_root_"+className+"Native.cpp");
      jnicpp_file << jnicpp << endl;
      jnicpp_file.close();

      delete appliedArgs;
      delete mcounts;
      delete methodMap;
    } // クラスループ

    jroot += "}\n";
    
    h_file << "}" << endl;
    //jh_file << "}" << endl;
  
    h_file.close();
    cpp_file.close();
    //jh_file.close();
    
    ofstream jroot_file(java_output_dir + "/JRoot.java");
    jroot_file << jroot << endl;
    jroot_file.close();
    
    return 0;
  } catch( TString msg ) {
    cerr << "[ERROR] " << msg << "\n  at main()" << endl;
  }
};
