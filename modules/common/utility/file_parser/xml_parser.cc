#include "xml_parser.h"

#include <iostream>

namespace atd {
namespace utility {

int XMLParser::Insert2DTable(const char* xml_path, const char* table_ID,
                             const MonotonicDiscreteLine2D& table) {
  XMLDocument doc;
  XMLError load_code = doc.LoadFile(xml_path);
  if (load_code) {
    XMLDeclaration* declaration = doc.NewDeclaration();
    doc.InsertFirstChild(declaration);
  }

  auto table_storage = doc.NewElement("Table2DContent");
  table_storage->SetAttribute("TableID", table_ID);
  doc.InsertEndChild(table_storage);

  for (auto point4loop : table.get_PointSet()) {
    auto table_content = doc.NewElement("Point2D");
    table_content->SetAttribute("x", point4loop.get_x_());
    table_content->SetAttribute("y", point4loop.get_y_());
    table_storage->InsertEndChild(table_content);
  }

  return doc.SaveFile(xml_path);
}

int XMLParser::Import2DTable(const char* xml_path, const char* table_ID,
                             MonotonicDiscreteLine2D& table) {
  XMLDocument doc;
  XMLError load_code = doc.LoadFile(xml_path);
  if (load_code) {
    return load_code;
  }
  auto current_element = doc.RootElement();
  while (current_element) {
    if (strcmp(current_element->Name(), "Table2DContent")) {
      current_element = current_element->NextSiblingElement("Table2DContent");
      continue;
    }
    if (!strcmp(current_element->FirstAttribute()->Value(), table_ID)) {
      table.clear();
      auto point_element = current_element->FirstChildElement("Point2D");
      while (point_element) {
        table.get_Mutable_PointSet()->emplace_back(
            point_element->FirstAttribute()->DoubleValue(),
            point_element->FirstAttribute()->Next()->DoubleValue());
        point_element = point_element->NextSiblingElement("Point2D");
      }
      return XMLError::XML_SUCCESS;
    }
    current_element = current_element->NextSiblingElement("Table2DContent");
  }
  return 100;  //  no match element
}

}  // namespace utility
}  // namespace atd