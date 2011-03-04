<?php
require_once ("data/encodeHTML.inc");

/**
 Provides style settings and script for the form. Call from the header
 */
function registrationFormHeader()
{
   ?>
<link
	href="regform.css" type="text/css" rel="stylesheet" />
<script
	type="text/javascript" src="regform.js"></script>
   <?php
}

/**
 Creates a form for editing registrant data.
 This form collects contact information and registration type.
 $record POST format registration data
 $catList an array of category ordered by competition class and competition catogory
 $action is html sanitized url for the http post
 */
function registrationForm($record, $catList, $action)
{
   $cat = $catList[$record['catID']];
   //debugArr('regform.php:category registered category data: ', $cat);
   $onClickString = 'onclick="setEnabledCompetitor(' .
   tfBool($cat, "hasTeamReg") . ',' .
   tfBool($cat, "hasStudentReg") . ',' .
   tfBool($cat, "hasFourMinute") . ')"';

   echo '<form id="regForm" class="regForm" action="' . $action . '" method="post" onreset="checkDisplay()" onsubmit="enableAllForPost()">' . "\n";

   echo '<table><tbody>' . "\n";
   echo '<tr>' . "\n";

   // Pilot information
   echo '<td class="form_text"><label for="input1">Given name:</label><input id="input1" onChange="copyRegToOwner()" name="givenName" value="' . $record["givenName"] . '" maxlength="72" size="32"/></td>' . "\n";
   echo '<td class="form_text"><label for="input2">Family name:</label><input id="input2" onChange="copyRegToOwner()" name="familyName" value="' . $record["familyName"] . '" maxlength="72" size="32"/></td>' . "\n";
   echo '<td colspan="2" class="form_text"><label for="input9">Email address:</label><input id="input9" name="email" value="' . $record["email"] . '" maxlength="320" size="60"/></td>' . "\n";
   echo '</tr><tr>' . "\n";
   echo '<td colspan="2" class="form_text"><label for="input4">Street address:</label><input id="input4" onChange="copyRegToOwner()" name="address" value="' . $record["address"] . '" maxlength="72" size="60"/></td>' . "\n";
   echo '</tr><tr>' . "\n";
   echo '<td class="form_text"><label for="input5">City:</label><input id="input5" onChange="copyRegToOwner()" name="city" value="' . $record["city"] . '" maxlength="24" size="18"/></td>' . "\n";
   echo '<td class="form_text"><label for="input6">State:</label><input id="input6" onChange="copyRegToOwner()" name="state" value="' . $record["state"] . '" maxlength="24" size="18"/></td>' . "\n";
   echo '</tr><tr>' . "\n";
   echo '<td class="form_text"><label for="input7">Country:</label><input id="input7" onChange="copyRegToOwner()" name="country" value="' . $record["country"] . '" maxlength="24" size="18"/></td>' . "\n";
   echo '<td class="form_text"><label for="input8">Postal code:</label><input id="input8" onChange="copyRegToOwner()" name="postalCode" value="' . $record["postalCode"] . '" maxlength="12" size="12"/></td>' . "\n";
   echo '<td class="form_text"><label for="input3">Cell phone number:</label><input id="input3" onChange="copyRegToOwner()" name="contactPhone" value="' . $record["contactPhone"] . '" maxlength="16" size="16"/></td>' . "\n";
   echo '</tr><tr>' . "\n";
   echo '<td class="form_text"><label for="input12">EAA Number:</label><input id="input12" name="eaaID" value="' . $record["eaaID"] . '" maxlength="12" size="12"/></td>' . "\n";
   echo '<td class="form_text"><label for="input13">IAC Number:</label><input id="input13" name="iacID" value="' . $record["iacID"] . '" maxlength="12" size="12"/></td>' . "\n";
   echo '<td class="form_text"><label for="shirtsize">Shirt Size:</label><select id="shirtsize" name="shirtsize">' . "\n";
   echo '<option value="XS" ' . isSelected($record, 'shirtsize', 'XS', 'selected') . '>Extra Small</option>' . "\n";
   echo '<option value="S" ' . isSelected($record, 'shirtsize', 'S', 'selected') . '>Small</option>' . "\n";
   echo '<option value="M" ' . isSelected($record, 'shirtsize', 'M', 'selected') . '>Medium</option>' . "\n";
   echo '<option value="L" ' . isSelected($record, 'shirtsize', 'L', 'selected') . '>Large</option>' . "\n";
   echo '<option value="XL" ' . isSelected($record, 'shirtsize', 'XL', 'selected') . '>Extra Large</option>' . "\n";
   echo '<option value="XXL" ' . isSelected($record, 'shirtsize', 'XXL', 'selected') . '>Double Extra Large</option>' . "\n";
   echo '</select></td>' . "\n";
   echo '</tr><tr>' . "\n";
   echo '<td class="form_text"><label for="iceName">Emergency contact name:</label><input id="iceName" name="iceName" value="' . $record["iceName"] . '"  maxlength="72" size="32"/></td>' . "\n";
   echo '<td class="form_text"><label for="icePhone1">First phone number:</label><input id="icePhone1" name="icePhone1" value="' . $record["icePhone1"] . '" maxlength="16" size="16"/></td>' . "\n";
   echo '<td class="form_text"><label for="icePhone2">Second phone number:</label><input id="icePhone2" name="icePhone2" value="' . $record["icePhone2"] . '" maxlength="16" size="16"/></td>' . "\n";
   echo '</tr></tbody></table>' . "\n";

   // Participant type (compType)
   echo '<span class="form_select"><label for="input16">Register as:</label><fieldset id="input16" legend="Register as">' . "\n";
   echo '<input ' . $onClickString . ' class="form_select" type="radio" name="compType" value="regrets" ' . isSelected($record, "compType", "regrets") . '>not attending</input>' . "\n";
   echo '<input ' . $onClickString . ' class="form_select" type="radio" name="compType" value="competitor" ' . isSelected($record, "compType", "competitor") . '>competitor</input>' . "\n";
   echo '<input ' . $onClickString . ' class="form_select" type="radio" name="compType" value="volunteer" ' . isSelected($record, "compType", "volunteer") . '>volunteer</input>' . "\n";
   echo '</fieldset></span>' . "\n";
   
   // Class (compClass)
   echo '<div id="competitor">' . "\n";
   echo '<span class="form_select"><label for="compCat">Competition category:</label><fieldset id="compCat" legend="Competition category">' . "\n";
   $col = 0;
   echo '<table><tbody><tr>';
   foreach ($catList as $cat)
   {
      if ($col != 0 && ($col % 5) == 0) echo '</tr><tr>';
      echo '<td><input onclick="setEnabledForCategory(' .
      tfBool($cat, "hasTeamReg") . ',' .
      tfBool($cat, "hasStudentReg") . ',',
      tfBool($cat, "hasFourMinute") . ')" class="form_select" type="radio" name="catID" value="' .
      $cat['catID'] . '" ' . isSelected($record, 'catID', $cat['catID']) . '>' . $cat['name'] .
        '</input></td>' . "\n";
      ++$col;
   }
   echo '</tr></tbody></table>';
   echo '</fieldset></span>' . "\n";

   // Team option
   echo '<div class="extra" id="team">';
   echo '<p class="form_line">';
   echo '<span class="form_boolean"><input  class="form_boolean" type="checkbox" name="teamAspirant"' .
   boolChecked($record, "teamAspirant") . '>Register as a world team aspirant</input></span>' . "\n";
   echo '</p><p class="form_line">';
   echo '<div id="teamSelected">';
   echo '<td class="form_text"><label for="input14">FAI Sporting License:</label><input id="input14" name="faiID" value="' . $record["faiID"] . '" maxlength="12" size="12"/></td>' . "\n";
   echo '</p></div></div>';

   // Four minute free option
   echo '<div class="extra" id="fourMinute">';
   echo '<p class="form_line">';
   echo '<span class="form_boolean"><input  class="form_boolean" type="checkbox" name="fourMinFree"' .
   boolChecked($record, "fourMinFree") . '>Register for the four minute free program</input></span>' . "\n";
   echo '</p><p class="form_line">';
   echo '</div>';

   // Student option
   echo '<div class="extra" id="student">';
   echo '<p class="form_line">';
   echo '<span class="form_boolean"><input  class="form_boolean" type="checkbox" name="isStudent"' .
   boolChecked($record, "isStudent") . '>Register as a student</input></span>' . "\n";
   echo '</p><p class="form_line">';
   echo '<div id="studentSelected">';
   echo '<span class="form_text"><label for="accred">Name of accredited university or FAA approved A&amp;P program:</label><input id="accred" name="university" value="' . $record["university"] . '" maxlength="48" size="48"/></span>' . "\n";
   echo '</p><p class="form_line">';
   echo '<span class="form_text"><label for="program">Undergraduate program of study:</label><input id="program" name="program" value="' . $record["program"] . '" maxlength="32" size="32"/></span>' . "\n";
   echo '</p></div></div></div>';

   // Submission
   echo '<div class="regSubmit">' . "\n";
   echo '<div id="volunteer">';
   echo '<input class="submit" name="submit" type="submit" value="Save and Continue Registration"/>' . "\n";
   echo '</div>';
   echo '<input class="submit" name="save" type="submit" value="Save and Return"/>' . "\n";
   echo '</div></td>' . "\n";

   // Owner fields (all hidden)
   echo "<input type=\"hidden\" class=\"form_boolean\" id=\"in_ownerPilot\" onClick=\"checkOwnerPilot()\" type=\"checkbox\" name=\"ownerPilot\" " . boolChecked($record, 'ownerPilot') . "/>\n";
   echo "<input type=\"hidden\" id=\"in_ownerName\" name=\"ownerName\" value=\"" . $record['ownerName'] . "\" maxlength=\"72\" size=\"60\"/>\n";
   echo "<input type=\"hidden\" id=\"in_ownerPhone\" name=\"ownerPhone\" value=\"" . $record['ownerPhone'] . "\" maxlength=\"16\" size=\"16\"/>\n";
   echo "<input type=\"hidden\" id=\"in_ownerAddress\" name=\"ownerAddress\" value=\"" . $record['ownerAddress'] . "\" maxlength=\"72\" size=\"60\"/>\n";
   echo "<input type=\"hidden\" id=\"in_ownerCity\" name=\"ownerCity\" value=\"" . $record['ownerCity'] . "\" maxlength=\"24\" size=\"18\"/>\n";
   echo "<input type=\"hidden\" id=\"in_ownerState\" name=\"ownerState\" value=\"" . $record['ownerState'] . "\" maxlength=\"24\" size=\"18\"/>\n";
   echo "<input type=\"hidden\" id=\"in_ownerPostal\" name=\"ownerPostal\" value=\"" . $record['ownerPostal'] . "\" maxlength=\"12\" size=\"12\"/>\n";
   echo "<input type=\"hidden\" id=\"in_ownerCountry\" name=\"ownerCountry\" value=\"" . $record['ownerCountry'] . "\" maxlength=\"24\" size=\"18\"/>\n";

   echo '</form>';
}
?>
<?php
/*
 Copyright 2008 International Aerobatic Club, Inc.

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
