<?php
/*  makeformXlate.inc, acrs/include/ui, dclo, 10/24/2010
 *  contest table query functions
 *
 *  Changes:
 *    10/24/2010 jim_ward       correct ref to uninitialized var $fail.
 */

require_once ('data/encodeHTML.inc');
require_once ('data/encodeSQL.inc');
require_once ('ui/cform2.inc');
require_once ("ui/make_form2.inc");

function xlateDataForPDF($db_conn, $record)
{
   // debugArr('xlateDataForPDF has ', $record);
   $startDate = strtotime($record['startDate']);
   $endDate = strtotime($record['endDate']);
   $fields = array ();
   $fields['contest_name'] = $record['name'];
   $fields['contest_date'] = date('M j', $startDate) . ' - ' . date('M j Y', $endDate);
   $fields['contest_location'] = $record['location'];
   $fields['contest_type'] = ucwords($record['class']);
   $fields['compType'] = $record['compType'];
   $fields['contest_category'] = ucwords($record['category']);
   $fields['pilot_first_name'] = $record['givenName'];
   $fields['pilot_last_name'] = $record['familyName'];
   $fields['pilot_iac_number'] = $record['iacID'];
   $fields['pilot_street'] = $record['address'];
   $fields['pilot_city'] = $record['city'];
   $fields['pilot_state'] = $record['state'];
   $fields['pilot_country'] = $record['country'];
   $fields['pilot_zip'] = $record['postalCode'];
   $fields['pilot_email'] = $record['email'];
   //    $fields['pilot_birthdate'] = $record[''];
   //    $fields['pilot_phone'] = $record['contactPhone'];
   $fields['pilot_cell'] = $record['contactPhone'];
   $fields['pilot_chapter'] = $record['chapter'];
   $fields['pilot_fai_number'] = $record['faiID'];
   $fields['pilot_license_type'] = $record['certType'];
   $fields['pilot_license_number'] = $record['certNumber'];
   $fields['pilot_emergency_contact'] = $record['iceName'];
   $fields['pilot_emergency_contact_phone'] = $record['icePhone1'];
   if ($record['icePhone2'] != '') $fields['pilot_emergency_contact_phone'] .= ' or ' . $record['icePhone2'];
   $fields['aircraft_insurance_company'] = $record['insCompany'];
   $fields['aircraft_insurance_type'] = $record['liabilityAmt'] == 'y' ? 'Liability' : '';
   $fields['aircraft_insurance_amount'] = $record['liabilityAmt'] == 'y' ? '$1,000,000' : 'none';;
   $fields['owner_name'] = $record['ownerName'];
   $fields['owner_phone'] = $record['ownerPhone'];
   $fields['owner_street'] = $record['ownerAddress'];
   $fields['owner_city'] = $record['ownerCity'];
   $fields['owner_state'] = $record['ownerState'];
   $fields['owner_zip'] = $record['ownerPostal'];
   $fields['aircraft_make_model'] = $record['airplaneMake']. ' ' . $record['airplaneModel'];
   $fields['aircraft_n_number'] = $record['airplaneRegID'];
   $fields['aircraft_color'] = $record['airplaneColors'];
   $fields['aircraft_engine_make'] = $record['engineMake'];
   $fields['aircraft_engine_model'] = $record['engineModel'];
   $fields['aircraft_engine_horsepower'] = $record['engineHP'];
   $fields['aircraft_airworthiness'] = ucwords($record['airworthiness']);
   xLateOrderOfFlightFields($db_conn, $record['regID'], $record['ctstID'], $record['airplaneRegID'], $fields);
   $fields['volunteer_safety_pilot_for'] = $record['safety'];
   xLateVolunteerFields($db_conn, $record['regID'], $record['ctstID'], $record['userID'], $fields);
   //debugArr('fields has ', $fields);

   return $fields;
}

function xLateOrderOfFlightFields($db_conn, $regID, $ctstID, $airplaneRegID, &$fields)
{
   $fields['aircraft_other_who_else'] = '';
   $fields['aircraft_other_category'] = '';
   $fields['aircraft_other'] = 'No';
   $query = 'select givenName, familyName, d.category'.
    ' from registrant a, registration b, reg_type c, ctst_cat d where'.
    ' a.userID = c.userID and b.regID = c.regID and c.ctstID = ' . intSQL($ctstID) .
    ' and upper(b.airplaneRegID) = ' . strSQL(strtoupper($airplaneRegID), 16) . 
    ' and b.regID <> ' . intSQL($regID) .
    ' and d.catID = b.catID';
   //debug('make_formXlate oof query: '. $query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      notifyError(dbErrorText(), 'reportRegIAC');
   } else
   {
      $curRcd = dbFetchAssoc($result);
      if ($curRcd !== false)
      {
         $fields['aircraft_other'] = 'Yes';
         $first = true;
         while ($curRcd !== false)
         {
            if ($first)
            {
               $first = false;
            }
            else
            {
               $fields['aircraft_other_who_else'] .= ', ';
               $fields['aircraft_other_category'] .= ', ';
            }
            $pn = substr($curRcd['givenName'], 0, 1) . '. ' . $curRcd['familyName'];
            $fields['aircraft_other_who_else'] .= $pn;
            $fields['aircraft_other_category'] .= ucwords(substr($curRcd['category'],0,3));
            $curRcd = dbFetchAssoc($result);
         }
      }
   }
}

function xLateVolunteerFields($db_conn, $regID, $ctstID, $userID, &$fields)
{
   $volMap = array(
   'judge' => 'volunteer_judge',
   'assistJudge' => 'volunteer_assist', 
   'recorder' => 'volunteer_recorder', 
   'boundary' => 'volunteer_boundary',
   'runner' => 'volunteer_runner', 
   'deadline' => 'volunteer_deadline', 
   'timer' => 'volunteer_timer', 
   'assistChief' => 'volunteer_assistChief'
   );
   $query = 'select a.volunteer, c.category, c.class '.
    ' from volunteer a, registration b, ctst_cat c where'.
    ' a.userID = ' . intSQL($userID) .
    ' and b.regID = ' . intSQL($regID) .
    ' and c.ctstID = ' . intSQL($ctstID) .
    ' and b.catID <> a.catID ' .
    ' and a.catID = c.catID' .
    ' order by a.volunteer, c.catID';
   //debug('make_formXlate vol query: '. $query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      notifyError(dbErrorText(), 'reportRegIAC');
   } else
   {
      $curRcd = dbFetchAssoc($result);
      while ($curRcd !== false)
      {
         //debugArr('xLateVolunteerFields query result', $curRcd);
         $volunteer = $curRcd['volunteer'];
         $cat = strtoupper(substr($curRcd['class'], 0, 1)) . '.' . ucwords(substr($curRcd['category'], 0, 3));
         $volTypes = explode(",", $volunteer);
         foreach ($volTypes as $volType)
         {
            $vf = $volMap[$volType];
            if ($fields[$vf] == '')
            {
               $fields[$vf] = $cat;
            }
            else
            {
               $fields[$vf] .= ', '.$cat;
            }
         }
         $curRcd = dbFetchAssoc($result);
      }
   }
}

function writePilots($db_conn, $result)
{
   $form = new CForm();
   $index = 1;
   $curRcd = dbFetchAssoc($result);
   while ($curRcd !== false)
   {
      $fields = xlateDataForPDF($db_conn, $curRcd);
      Make_one_registration($fields, $form, $index);
      $curRcd = dbFetchAssoc($result);
      ++ $index;
   }
   Output_Form($form);
}

function generateIACRegFormPilots($db_conn, $ctstID, $userID,
  $thisPilotOnly = false, $offset = 0, $count = 0)
{
   $query = 'select a.userID, f.regID, f.ctstID, b.name, startDate, endDate, location, class, category,' .
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
   if ($thisPilotOnly) $query .= ' a.userID = ' . intSQL($userID) . ' and';
   $query .= ' f.userID = a.userID ' .
    ' and f.ctstID = ' . intSQL($ctstID) .
    ' and c.regID = f.regID' .
    ' and b.ctstID = f.ctstID' .
    ' and e.catID = c.catID' .
    " and f.compType = 'competitor'" .
    ' order by e.category, a.familyName, a.givenName';
   if (0 < $count) {
     $endOffset = $offset + $count;
     $query .= " limit $offset,$endOffset";
   }
   $query .= ';';
   //debug('make_formXlate.generateIACRegFormPilots query' . $query);
   $fail = '';
   $result = dbQuery($db_conn, $query);
   if ($result === false)
      $fail = notifyError(dbErrorText(), 'reportRegIAC');
   else
      writePilots($db_conn, $result);
   return $fail;
}

/*
 Copyright 2008, 2010 International Aerobatic Club, Inc.

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
