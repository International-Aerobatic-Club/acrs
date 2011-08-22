<?php
/*  make_VolfromXlate.inc, acrs/include/ui, dclo, 08/22/2011
 *
 */

require_once ('data/encodeHTML.inc');
require_once ('data/encodeSQL.inc');
require_once ('ui/cform2.inc');
require_once ("ui/make_form2.inc");
require_once ("ui/make_formXlate.inc");

function writeVolunteers($db_conn, $result)
{
   $form = new CForm();
   $index = 1;
   $curRcd = dbFetchAssoc($result);
   while ($curRcd !== false)
   {
      $fields = xlateDataForPDF($db_conn, $curRcd);
      Make_one_volunteer($fields, $form, $index);
      $curRcd = dbFetchAssoc($result);
      ++ $index;
   }
   Output_Form($form);
}

function generateIACVolunteerForms($db_conn, $ctstID)
{
   $query = 'select a.userID, f.regID, f.ctstID, b.name, startDate,' .
        ' endDate, location, class, category, compType,' .
    		' givenName, familyName, iacID, address, city, state, country, postalCode,' .
    		' email, contactPhone, c.chapter, faiID, certType, certNumber, safety,' .
            ' iceName, icePhone1, icePhone2,' .
    		' insCompany, liabilityAmt,' .
    		' ownerName, ownerPhone, ownerAddress, ownerCity, ownerState,' .
    		' ownerCountry, ownerPostal, airplaneMake, airplaneModel, airplaneRegID,' .
            ' airplaneColors, engineMake, engineModel, engineHP,' .
    		' airworthiness' .
    		' from registrant a, contest b, registration c, ctst_cat e, reg_type f' .
            ' where'; 
   $query .= ' f.userID = a.userID ' .
    ' and f.ctstID = ' . intSQL($ctstID) .
    ' and c.regID = f.regID' .
    ' and b.ctstID = f.ctstID' .
    ' and e.catID = c.catID' .
    " and f.compType in ('competitor','volunteer')" .
    ' order by e.category, a.familyName, a.givenName;';
   debug('make_VolformXlate.generateIACVolunteerForms query' . $query);
   $fail = '';
   $result = dbQuery($db_conn, $query);
   if ($result === false)
      $fail = notifyError(dbErrorText(), 'reportRegIAC');
   else
      writeVolunteers($db_conn, $result);
   return $fail;
}

/*
 Copyright 2011 International Aerobatic Club, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */
?>
