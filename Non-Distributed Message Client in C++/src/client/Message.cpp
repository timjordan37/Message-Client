#include "Message.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

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

Message::Message(){
  toUser="Null";
  fromUser="Null";
  subject="Null";
  date="Null";
  msgBody="Null";
}

Message::Message(string toAUser, string fromAUser, string aSubject,
		 string aDate, string aMsg) {
  toUser = toAUser;
  fromUser = fromAUser;
  subject = aSubject;
  date = aDate;
  msgBody = aMsg;
}

void Message::print(){
  cout<<toUser<<endl;
}
