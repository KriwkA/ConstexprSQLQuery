//#include <iostream>
#include "core/insert_or_update.hpp"

namespace  {

class Table {
public:
   static constexpr ctll::fixed_string TABLE_NAME = L"Table";
};


struct Model {
   int field1 = 0;
   double field2 = 0;
   float field3 = 0;
   std::string field4 = "str";
};

}

template<>
struct reflection::ModelMetaInfo<Model> {
   static constexpr FieldInfo f1{L"field123412341"};
   static constexpr FieldInfo f2{L"field2"};
   static constexpr FieldInfo f3{L"field3"};
   static constexpr FieldInfo f4{L"field4"};

   using FieldsInfo = reflection::FieldsMetaInfo<
      &Model::field1, &f1,
      &Model::field2, &f2,
      &Model::field3, &f3,
      &Model::field4, &f4
   >;
};


int main()
{      
   constexpr auto q = db::InsertQuery<Model, Table>();
   constexpr auto q2 = db::UpdateQuery<Model, Table>();

   std::wcout << q << std::endl;
   std::wcout << q2 << std::endl;

   return 0;
}
