// @HEADER
// ***********************************************************************
// 
//                    Teuchos: Common Tools Package
//                 Copyright (2004) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Michael A. Heroux (maherou@sandia.gov) 
// 
// ***********************************************************************
// @HEADER

#include "Teuchos_VerboseObject.hpp"
#include "Teuchos_XMLParameterListHelpers.hpp"
#include "Teuchos_StandardDependencies.hpp"
#include "Teuchos_DependencySheet.hpp"
#include "Teuchos_StandardConditions.hpp"
#include "Teuchos_StandardDependencies.hpp"
#include "Teuchos_UnitTestHarness.hpp"
#include "Teuchos_DependencyXMLConverterDB.hpp"
#include "Teuchos_StandardDependencyXMLConverters.hpp"
#include "Teuchos_ParameterList.cpp"


namespace Teuchos{


TEUCHOS_UNIT_TEST(Teuchos_Dependencies, stringVisualDepTest){
  std::string dependee1 = "string param";
  std::string dependee2 = "string param2";
  std::string dependent1 = "dependent param1";
  std::string dependent2 = "dependent param2";
  ParameterList myDepList("String Visual Dep List");
  RCP<DependencySheet> myDepSheet = rcp(new DependencySheet);
  myDepList.set(dependee1, "val1");
  myDepList.set(dependee2, "val2");
  myDepList.set(dependent1, 1.0);
  myDepList.set(dependent2, 1.0);

  StringVisualDependency::ValueList valList1 = tuple<std::string>("val1");

  RCP<StringVisualDependency> basicStringVisDep = rcp(
    new StringVisualDependency(
      myDepList.getEntryRCP(dependee1),
      myDepList.getEntryRCP(dependent1),
      valList1));

  Dependency::ParameterEntryList dependentList;
  dependentList.insert(myDepList.getEntryRCP(dependent1));
  dependentList.insert(myDepList.getEntryRCP(dependent2));
  StringVisualDependency::ValueList valList2 = 
    tuple<std::string>("val1", "val2");

  RCP<StringVisualDependency> complexStringVisDep = rcp(
    new StringVisualDependency(
      myDepList.getEntryRCP(dependee2),
      dependentList,
      valList2,
      false));

  myDepSheet->addDependency(basicStringVisDep);
  myDepSheet->addDependency(complexStringVisDep);

  RCP<DependencySheet> readInDepSheet = rcp(new DependencySheet);

  XMLParameterListWriter plWriter;
  XMLObject xmlOut = plWriter.toXML(myDepList, myDepSheet);
  out << xmlOut.toString();

  RCP<ParameterList> readInList = 
    writeThenReadPL(myDepList, myDepSheet, readInDepSheet); 

  RCP<ParameterEntry> readinDependee1 = readInList->getEntryRCP(dependee1);
  RCP<ParameterEntry> readinDependent1 = readInList->getEntryRCP(dependent1);
  RCP<ParameterEntry> readinDependee2 = readInList->getEntryRCP(dependee2);
  RCP<ParameterEntry> readinDependent2 = readInList->getEntryRCP(dependent2);
  
  RCP<Dependency> readinDep1 =
    *(readInDepSheet->getDependenciesForParameter(readinDependee1)->begin());

  RCP<Dependency> readinDep2 =
    *(readInDepSheet->getDependenciesForParameter(readinDependee2)->begin());

  std::string stringVisXMLTag = 
    DummyObjectGetter<StringVisualDependency>::getDummyObject()->getTypeAttributeValue();

  TEST_ASSERT(readinDep1->getTypeAttributeValue() == stringVisXMLTag);
  TEST_ASSERT(readinDep2->getTypeAttributeValue() == stringVisXMLTag);

  TEST_ASSERT(readinDep1->getFirstDependee().get() == readinDependee1.get());
  TEST_ASSERT(readinDep1->getDependents().size() == 1);
  TEST_ASSERT((*readinDep1->getDependents().begin()).get() == readinDependent1.get());

  TEST_ASSERT(readinDep2->getFirstDependee().get() == readinDependee2.get());
  TEST_ASSERT(readinDep2->getDependents().size() == 2);
  TEST_ASSERT(
    readinDep2->getDependents().find(readinDependent1) 
    !=
    readinDep2->getDependents().end()
  );
  TEST_ASSERT(
    readinDep2->getDependents().find(readinDependent2)
    !=
    readinDep2->getDependents().end()
  );
    
  RCP<StringVisualDependency> castedDep1 =
    rcp_dynamic_cast<StringVisualDependency>(readinDep1);
  RCP<StringVisualDependency> castedDep2 =
    rcp_dynamic_cast<StringVisualDependency>(readinDep2);

  TEST_COMPARE_ARRAYS(
    castedDep1->getValues(), basicStringVisDep->getValues());
  TEST_COMPARE_ARRAYS(
    castedDep2->getValues(), complexStringVisDep->getValues());

  TEST_EQUALITY(castedDep1->getShowIf(), basicStringVisDep->getShowIf());
  TEST_EQUALITY(castedDep2->getShowIf(), complexStringVisDep->getShowIf());
  

}


} //namespace Teuchos

