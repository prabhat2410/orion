/*
*
* Copyright 2013 Telefonica Investigacion y Desarrollo, S.A.U
*
* This file is part of Orion Context Broker.
*
* Orion Context Broker is free software: you can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* Orion Context Broker is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero
* General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with Orion Context Broker. If not, see http://www.gnu.org/licenses/.
*
* For those usages not covered by this license please contact with
* iot_support at tid dot es
*
* Author: Ken Zangelin
*/
#include <string>
#include <vector>

#include "common/globals.h"

#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"

#include "jsonParse/JsonNode.h"
#include "parse/nullTreat.h"
#include "jsonParse/jsonNotifyContextAvailabilityRequest.h"

#include "rest/ConnectionInfo.h"



/* ****************************************************************************
*
* subscriptionId -
*/
static std::string subscriptionId(const std::string& path, const std::string& value, ParseData* parseDataP)
{
  LM_T(LmtParse, ("Got a subscriptionId: '%s'", value.c_str()));
  parseDataP->ncar.res.subscriptionId.set(value);
  return "OK";
}



/* ****************************************************************************
*
* contextRegistrationResponse -
*/
static std::string contextRegistrationResponse(const std::string& path, const std::string& value, ParseData* parseDataP)
{
  LM_T(LmtParse, ("Got a contextRegistrationResponse"));

  parseDataP->ncar.crrP = new ContextRegistrationResponse();
  parseDataP->ncar.res.contextRegistrationResponseVector.push_back(parseDataP->ncar.crrP);

  return "OK";
}



/* ****************************************************************************
*
* entityId -
*/
static std::string entityId(const std::string& path, const std::string& value, ParseData* parseDataP)
{
  LM_T(LmtParse, ("Got an entityId"));

  parseDataP->ncar.entityIdP = new EntityId();
  parseDataP->ncar.crrP->contextRegistration.entityIdVector.push_back(parseDataP->ncar.entityIdP);

  return "OK";
}



/* ****************************************************************************
*
* entityIdId -
*/
static std::string entityIdId(const std::string& path, const std::string& value, ParseData* parseDataP)
{
  LM_T(LmtParse, ("Got an entityId.Id: '%s'", value.c_str()));

  parseDataP->ncar.entityIdP->id = value;
  return "OK";
}



/* ****************************************************************************
*
* entityIdType -
*/
static std::string entityIdType(const std::string& path, const std::string& value, ParseData* parseDataP)
{
  LM_T(LmtParse, ("Got an entityId.Type: '%s'", value.c_str()));

  parseDataP->ncar.entityIdP->type = value;
  return "OK";
}



/* ****************************************************************************
*
* entityIdIsPattern -
*/
static std::string entityIdIsPattern(const std::string& path, const std::string& value, ParseData* parseDataP)
{
  LM_T(LmtParse, ("Got an entityId.IsPattern: '%s'", value.c_str()));

  parseDataP->ncar.entityIdP->isPattern = value;
  return "OK";
}



/* ****************************************************************************
*
* attribute -
*/
static std::string attribute(const std::string& path, const std::string& value, ParseData* parseDataP)
{
  LM_T(LmtParse, ("got an attribute"));

  parseDataP->ncar.craP = new ContextRegistrationAttribute();
  parseDataP->ncar.crrP->contextRegistration.contextRegistrationAttributeVector.push_back(parseDataP->ncar.craP);
  return "OK";
}



/* ****************************************************************************
*
* attributeName -
*/
static std::string attributeName(const std::string& path, const std::string& value, ParseData* parseDataP)
{
  LM_T(LmtParse, ("got an attributeName: '%s'", value.c_str()));

  parseDataP->ncar.craP->name = value;
  return "OK";
}



/* ****************************************************************************
*
* attributeType -
*/
static std::string attributeType(const std::string& path, const std::string& value, ParseData* parseDataP)
{
  LM_T(LmtParse, ("got an attributeType: '%s'", value.c_str()));

  parseDataP->ncar.craP->type = value;
  return "OK";
}



/* ****************************************************************************
*
* providingApplication -
*/
static std::string providingApplication(const std::string& path, const std::string& value, ParseData* parseDataP)
{
  LM_T(LmtParse, ("Got a providingApplication: '%s'", value.c_str()));

  parseDataP->ncar.crrP->contextRegistration.providingApplication.set(value);
  return "OK";
}


#define CRR "/contextRegistrationResponses/contextRegistrationResponse"
/* ****************************************************************************
*
* jsonNcarParseVector -
*/
JsonNode jsonNcarParseVector[] =
{
  { "/subscriptionId",                                                         subscriptionId               },
  { "/contextRegistrationResponses",                                           jsonNullTreat                },
  { CRR,                                                                       contextRegistrationResponse  },

  { CRR "/contextRegistration",                                                jsonNullTreat                },

  { CRR "/contextRegistration/entities",                                       jsonNullTreat                },
  { CRR "/contextRegistration/entities/entity",                                entityId                     },
  { CRR "/contextRegistration/entities/entity/id",                             entityIdId                   },
  { CRR "/contextRegistration/entities/entity/type",                           entityIdType                 },
  { CRR "/contextRegistration/entities/entity/isPattern",                      entityIdIsPattern            },

  { CRR "/contextRegistration/attributes",                                     jsonNullTreat                },
  { CRR "/contextRegistration/attributes/attribute",                           attribute                    },
  { CRR "/contextRegistration/attributes/attribute/name",                      attributeName                },
  { CRR "/contextRegistration/attributes/attribute/type",                      attributeType                },

  { CRR "/contextRegistration/providingApplication",                           providingApplication         },

  { "LAST", NULL }
};



/* ****************************************************************************
*
* jsonNcarInit -
*/
void jsonNcarInit(ParseData* parseDataP)
{
  jsonNcarRelease(parseDataP);

  parseDataP->ncar.crrP                 = NULL;
  parseDataP->ncar.entityIdP            = NULL;
  parseDataP->ncar.craP                 = NULL;
  parseDataP->ncar.attributeMetadataP   = NULL;
  parseDataP->ncar.regMetadataP         = NULL;
}



/* ****************************************************************************
*
* jsonNcarRelease -
*/
void jsonNcarRelease(ParseData* parseDataP)
{
  parseDataP->ncar.res.release();
}



/* ****************************************************************************
*
* jsonNcarCheck -
*/
std::string jsonNcarCheck(ParseData* parseDataP, ConnectionInfo* ciP)
{
  return parseDataP->ncar.res.check(ciP->apiVersion, parseDataP->errorString);
}
