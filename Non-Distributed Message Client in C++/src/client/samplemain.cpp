#include "MessageGui.cpp"
#include "Library.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Check_Browser.H>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdlib.h>

using namespace std;

/**
 * Copyright (c) 2019 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ FLTK client UI for Message management.
 * This class extends the Gui component class MessageGui and demonstrates
 * sample control functions that respond to button clicks drop-down selects.
 * This software is meant to run on Linux and MacOS using g++.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @file    samplemain.cpp
 * @date    January, 2019
 **/

/*
 * Copyright 2019 Tim Jordan,
 *
 * This software is the intellectual property of the author,
 * and can not be distributed, used, copied, or
 * reproduced, in whole or in part, for any purpose, commercial
 * or otherwise. The author grants the ASU
 * Software Engineering program the right to copy, execute,
 * and evaluate this work for the purpose of
 * determining performance of the author in coursework, and for
 * Software Engineering program evaluation,
 * so long as this copyright and right-to-use statement is kept
 * in-tact in such use.
 * All other uses are prohibited and reserved to the author.
 *
 * Purpose: A message class.
 *
 * Ser321 Principles of Distributed Software Systems
 * @author Tim Jordan tsjorda1@asu.edu
 * @version September 2019
 */
class SampleStudentClient : public MessageGui {



   /** ClickedX is one of the callbacks for GUI controls.
    * Callbacks need to be static functions. But, static functions
    * cannot directly access instance data. This program uses "userdata"
    * to get around that by passing the instance to the callback
    * function. The callback then accesses whatever GUI control object
    * that it needs for implementing its functionality.
    */
   static void ClickedX(Fl_Widget * w, void * userdata) {
      std::cout << "You clicked Exit" << std::endl;
      exit(1);
   }

   static void ClickedGet(Fl_Widget * w, void * userdata) {

     Library * library = new Library();
  library->initLibraryFromJsonFile("messages.json");
      // since this is a static method, it does not have access to any instance variables.
      // get access through the userdata argument which is the instance.
      SampleStudentClient* anInstance = (SampleStudentClient*)userdata;
      std::cout << "You clicked Get" << std::endl;
      Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
      Fl_Input * dateFI = anInstance->dateFI;
      Fl_Input * subjectFI = anInstance->subjectFI;
      Fl_Input * toFI = anInstance->toFI;
      Fl_Input * fromFI = anInstance->fromFI;
      Fl_Check_Browser * headers = anInstance->headerFCB;

      // clear all existing entries from the list of message headers
      headers->clear();

      // lets setup some sample entries in the FCB
      // first get the date and place its string form into dateFI
      // get the time in a string
      auto aClk = std::chrono::system_clock::now();
      auto aTime = std::chrono::system_clock::to_time_t(aClk);
      std::string aDateStr(ctime(&aTime));
      // the conversion to string adds a new line at the end of the string. Trim it.
      std::string trimmed(trimMe(aDateStr));
      // load the date field with the current time in string form
      //dateFI->value(ctime(&aTime));
      dateFI->value(trimmed.c_str());
      std::cout << "date un-trimmed: "<<aDateStr<<". Date from the UI control: "
                << dateFI->value()<<std::endl;
      // get the instance's userId string so we know who is sending messages
      std::string userId = anInstance->userId;
      // all FLTK controls are loaded via c strings, not c++ string objects.

      for(std::map<string,Message>::iterator i =library->messages.begin();
	  i!= library->messages.end(); i++){
	string key = i->first;
	Message msg = library->messages[key];
	headers->add((msg.toUser+"     "+msg.date).c_str());
      }

      headers->checked(1,1);
      // fill in the corresponding fields as if we are displaying a real message
      toFI->value(library->messages["1"].toUser.c_str());
      fromFI->value(library->messages["1"].fromUser.c_str());
      dateFI->value(library->messages["1"].date.c_str());
      subjectFI->value(library->messages["1"].subject.c_str());
      messageFMI->value(library->messages["1"].msgBody.c_str());
  }

   static void ClickedDelete(Fl_Widget * w, void * userdata) {
     //
     //find out how to get the index of the map
     //show I can use that to delete
     //because key and index wont match after deleting
     //
     //

     SampleStudentClient* anInstance = (SampleStudentClient*)userdata;
     Fl_Check_Browser * headers = anInstance->headerFCB;
     Library * library = new Library();
     library->initLibraryFromJsonFile("messages.json");

     library->remove(headers->value());
     library->toJsonFile("messages.json");
     headers->clear();
     for(std::map<string,Message>::iterator i =library->messages.begin();
	 i!= library->messages.end(); i++){
       string key = i->first;
       Message msg = library->messages[key];
       headers->add((msg.toUser+"     "+msg.date).c_str());
     }
     headers->checked(1,1);
     std::cout << "You clicked Delete" << std::endl;
   }

   static std::string& trimMe (std::string& str) {
      // right trim
      while (str.length() > 0 && (str[str.length()-1] == ' ' || str[str.length()-1] == '\t'
                                  || str[str.length()-1] == '\n' ||  str[str.length()-1] == '\r')){
         str.erase(str.length ()-1, 1);
      }
      // left trim
      while (str.length () > 0 && (str[0] == ' ' || str[0] == '\t')){
         str.erase(0, 1);
      }
      return str;
   }

  static void ClickedReply(Fl_Widget * w, void * userdata) {
    cout<<"Clicked Reply"<<endl;
    SampleStudentClient* anInstance = (SampleStudentClient*)userdata;
      Fl_Check_Browser * headers = anInstance->headerFCB;
      Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
      Fl_Input * dateFI = anInstance->dateFI;
      Fl_Input * subjectFI = anInstance->subjectFI;
      Fl_Input * toFI = anInstance->toFI;
      Fl_Input * fromFI = anInstance->fromFI;

      string to, from, subject, message,date;
      to=toFI->value();
      from=fromFI->value();
      subject=subjectFI->value();
      auto aClk = std::chrono::system_clock::now();
      auto aTime = std::chrono::system_clock::to_time_t(aClk);
      std::string aDateStr(ctime(&aTime));
      std::string trimmed(trimMe(aDateStr));

      dateFI->value(trimmed.c_str());
      message=messageFMI->value();
      string re="Re:" , newline="\n\n\n";

      toFI->value(from.c_str());
      fromFI->value(to.c_str());
      subjectFI->value((re+subject).c_str());
      messageFMI->value((newline+message).c_str());
  }

  static void ClickedExport(Fl_Widget * w, void * userdata) {
    Library * library = new Library();
    library->initLibraryFromJsonFile("messages.json");
    library->toJsonFile("messages.json");
    cout<<"You clicked export"<<endl;
  }

   static void SelectedHeader(Fl_Widget * w, void * userdata) {
      SampleStudentClient* anInstance = (SampleStudentClient*)userdata;
      Fl_Check_Browser * headers = anInstance->headerFCB;
      Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
      Fl_Input * dateFI = anInstance->dateFI;
      Fl_Input * subjectFI = anInstance->subjectFI;
      Fl_Input * toFI = anInstance->toFI;
      Fl_Input * fromFI = anInstance->fromFI;

      Library * library = new Library();
      library->initLibraryFromJsonFile("messages.json");
      toFI->value(library->messages[std::to_string(headers->value())].toUser.c_str());
      fromFI->value(library->messages[std::to_string(headers->value())].fromUser.c_str());
      dateFI->value(library->messages[std::to_string(headers->value())].date.c_str());
      subjectFI->value(library->messages[std::to_string(headers->value())].subject.c_str());
      messageFMI->value(library->messages[std::to_string(headers->value())].msgBody.c_str());

      int howMany = headers->nitems();
      int which = headers->value();
      std::cout << "You selected a new message header. Index: " << which << std::endl;

      // unselect all but the item just selected. Note, first item is item 1 not 0
      for(int i=1; i<howMany+1; i++){
         if(i != which ){
            headers->checked(i,0);
	    // std::cout << "calling checked("<< i << ")" << std::endl;
         }
      }
      // std::cout << "You selected item " << which << " text " << headers->text(which)
      //      << "\nbrowser size is " << howMany
      //      << std::endl;
   }

  static void fillMenu(void * userdata){
    SampleStudentClient* anInstance = (SampleStudentClient*)userdata;
    Fl_Check_Browser * headers = anInstance->headerFCB;
    Library * library = new Library();
    library->initLibraryFromJsonFile("messages.json");
    for(std::map<string,Message>::iterator i =library->messages.begin();
	i!= library->messages.end(); i++){
      string key = i->first;
      Message msg = library->messages[key];
      headers->add((msg.toUser+"     "+msg.date).c_str());
    }
    headers->checked(1,1);
    Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
    Fl_Input * dateFI = anInstance->dateFI;
    Fl_Input * subjectFI = anInstance->subjectFI;
    Fl_Input * toFI = anInstance->toFI;
    Fl_Input * fromFI = anInstance->fromFI;
    toFI->value(library->messages["1"].toUser.c_str());
    fromFI->value(library->messages["1"].fromUser.c_str());
    dateFI->value(library->messages["1"].date.c_str());
    subjectFI->value(library->messages["1"].subject.c_str());
    messageFMI->value(library->messages["1"].msgBody.c_str());

  }

  static void ClickedSend(Fl_Widget * w, void * userdata){
    cout<<"You clicked send"<<endl;
    Library * library = new Library();
      library->initLibraryFromJsonFile("messages.json");
     SampleStudentClient* anInstance = (SampleStudentClient*)userdata;
    Fl_Check_Browser * headers = anInstance->headerFCB;
     Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
    Fl_Input * dateFI = anInstance->dateFI;
    Fl_Input * subjectFI = anInstance->subjectFI;
    Fl_Input * toFI = anInstance->toFI;
    Fl_Input * fromFI = anInstance->fromFI;
    string to, from, subject, message,date;
      to=toFI->value();
      from=fromFI->value();
      subject=subjectFI->value();
      auto aClk = std::chrono::system_clock::now();
      auto aTime = std::chrono::system_clock::to_time_t(aClk);
      std::string aDateStr(ctime(&aTime));
      std::string trimmed(trimMe(aDateStr));

      dateFI->value(trimmed.c_str());
      message=messageFMI->value();
      Message * msg = new Message(to,from,subject,trimmed,message);

      library->add(*msg);
      library->toJsonFile("messages.json");
  }

public:
   std::string userId;
   SampleStudentClient(const char * name = 0) : MessageGui(name) {
      getButt->callback(ClickedGet, (void*)this);
      deleteButt->callback(ClickedDelete, (void*)this);
      replyButt->callback(ClickedReply, (void*)this);
      sendButt->callback(ClickedSend, (void*)this);
      exportButt->callback(ClickedExport, (void*)this);
      headerFCB->callback(SelectedHeader, (void*)this);
      headerFCB->when(FL_WHEN_CHANGED);
      fillMenu((void*)this);

      callback(ClickedX);

      userId = "tsjorda1";
   }
};

int main() {

   SampleStudentClient cm("Tim Jordan's Message Browser");
   return (Fl::run());
}
