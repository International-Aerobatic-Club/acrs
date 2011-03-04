<?php
/*  flyingform.inc, acrs/include/form, dlco, 10/23/2010
 *  publish flying-info form to the user's browser
 *
 *  Changes:
 *    10/23/2010 jim_ward       remove Student certificate from user visibility; can't fly a contest with one
 *    10/23/2010 jim_ward       s/Current condition inspection/Current annual or condition inspection/.
 *                              Also, s/acrobatic/aerobatic/ for user consumption; the enums have not changed.
 */

require_once ("data/encodeHTML.inc");

/**
Provides style settings and script for the form. Call from the header
*/
function flyingFormHeader()
{
    echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
    echo '<script type="text/javascript" src="regform.js"></script>';
}

function flyingForm($record, $action)
{
    echo '<form id="regForm" class="regForm" action="' . $action . '" method="post" onreset="checkOwnerPilot()" onsubmit="enableAllForPost()">' . "\n";
    echo '<table><tbody>' . "\n";
    echo "<tr>\n";

    // Pilot certificate
    echo "<td rowspan=\"3\" class=\"form_select\">";
    echo "<label for=\"in_certType\">Pilot certificate type:</label><fieldset id=\"in_certType\" legend=\"Pilot certificate type\">\n";
    echo "<input class=\"form_select\" id=\"in_certType_sport\" type=\"radio\" name=\"certType\" value=\"sport\" " . isSelected($record, 'certType', 'sport') . ">Sport</input>\n";
    echo "<input class=\"form_select\" id=\"in_certType_recreational\" type=\"radio\" name=\"certType\" value=\"recreational\" " . isSelected($record, 'certType', 'recreational') . ">Recreational</input>\n";
    echo "<input class=\"form_select\" id=\"in_certType_private\" type=\"radio\" name=\"certType\" value=\"private\" " . isSelected($record, 'certType', 'private') . ">Private</input>\n";
    echo "<input class=\"form_select\" id=\"in_certType_commercial\" type=\"radio\" name=\"certType\" value=\"commercial\" " . isSelected($record, 'certType', 'commercial') . ">Commercial</input>\n";
    echo "<input class=\"form_select\" id=\"in_certType_atp\" type=\"radio\" name=\"certType\" value=\"atp\" " . isSelected($record, 'certType', 'atp') . ">ATP</input>\n";
    echo "</fieldset>\n"; //</td>\n";

    // cert number same cell
    //echo "<td class=\"form_text\">";
    echo "<label for=\"in_certNumber\">Certificate number:</label><input id=\"in_certNumber\" name=\"certNumber\" value=\"" . $record['certNumber'] . "\" maxlength=\"16\" size=\"16\"/>\n";
    echo "<label for=\"in_chapter\">Chapter number declaration:</label><input id=\"in_chapter\" name=\"chapter\" value=\"" . $record['chapter'] . "\" maxlength=\"6\" size=\"6\"/></td>\n";
    
    // medical, biannual,chute pack
    echo "<td class=\"form_boolean\"><input class=\"form_boolean\" id=\"in_currMedical\" type=\"checkbox\" name=\"currMedical\" " . boolChecked($record, 'currMedical') . ">Current medical</input></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_boolean\"><input class=\"form_boolean\" id=\"in_currBiAnn\" type=\"checkbox\" name=\"currBiAnn\" " . boolChecked($record, 'currBiAnn') . ">Current bi-annual</input></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_boolean\"><input class=\"form_boolean\" id=\"in_currPacked\" type=\"checkbox\" name=\"currPacked\" " . boolChecked($record, 'currPacked') . ">Current parachute pack</input></td>\n";

    echo "</tr><tr>\n";
    echo "<td colspan=\"2\" class=\"form_text\"><label for=\"in_safety\">Safety pilot name:</label><input id=\"in_safety\" name=\"safety\" value=\"" . $record['safety'] . "\" maxlength=\"72\" size=\"60\"/></td>\n";
    echo "</tr><tr>\n";
    
    // Owner
    echo "<td colspan=\"3\" class=\"form_boolean\"><input class=\"form_boolean\" id=\"in_ownerPilot\" onClick=\"checkOwnerPilot()\" type=\"checkbox\" name=\"ownerPilot\" " . boolChecked($record, 'ownerPilot') . ">Owner pilot</input></td>\n";
    echo "</tr><tr>\n";
    echo "<td colspan=\"2\" class=\"form_text\"><label for=\"in_ownerName\">Airplane owner name:</label><input id=\"in_ownerName\" name=\"ownerName\" value=\"" . $record['ownerName'] . "\" maxlength=\"72\" size=\"60\"/></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_ownerPhone\">Phone number:</label><input id=\"in_ownerPhone\" name=\"ownerPhone\" value=\"" . $record['ownerPhone'] . "\" maxlength=\"16\" size=\"16\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td colspan=\"2\" class=\"form_text\"><label for=\"in_ownerAddress\">Street address:</label><input id=\"in_ownerAddress\" name=\"ownerAddress\" value=\"" . $record['ownerAddress'] . "\" maxlength=\"72\" size=\"60\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_text\"><label for=\"in_ownerCity\">City:</label><input id=\"in_ownerCity\" name=\"ownerCity\" value=\"" . $record['ownerCity'] . "\" maxlength=\"24\" size=\"18\"/></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_ownerState\">State:</label><input id=\"in_ownerState\" name=\"ownerState\" value=\"" . $record['ownerState'] . "\" maxlength=\"24\" size=\"18\"/></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_ownerPostal\">Postal code:</label><input id=\"in_ownerPostal\" name=\"ownerPostal\" value=\"" . $record['ownerPostal'] . "\" maxlength=\"12\" size=\"12\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_text\"><label for=\"in_ownerCountry\">Country:</label><input id=\"in_ownerCountry\" name=\"ownerCountry\" value=\"" . $record['ownerCountry'] . "\" maxlength=\"24\" size=\"18\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_text\"><label for=\"in_airplaneMake\">Airplane make:</label><input id=\"in_airplaneMake\" name=\"airplaneMake\" value=\"" . $record['airplaneMake'] . "\" maxlength=\"24\" size=\"18\"/></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_airplaneModel\">Model:</label><input id=\"in_airplaneModel\" name=\"airplaneModel\" value=\"" . $record['airplaneModel'] . "\" maxlength=\"24\" size=\"18\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_select\"><label for=\"in_airworthiness\">Type of Airworthiness:</label><fieldset id=\"in_airworthiness\" legend=\"Type of Airworthiness\">\n";
    echo "<input class=\"form_select\" id=\"in_airworthiness_experimental\" type=\"radio\" name=\"airworthiness\" value=\"experimental\" " . isSelected($record, 'airworthiness', 'experimental') . ">experimental</input>\n";
    echo "<input class=\"form_select\" id=\"in_airworthiness_acrobatic\" type=\"radio\" name=\"airworthiness\" value=\"acrobatic\" " . isSelected($record, 'airworthiness', 'acrobatic') . ">aerobatic</input>\n";
    echo "</fieldset></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_airplaneRegID\">Registration:</label><input id=\"in_airplaneRegID\" name=\"airplaneRegID\" value=\"" . $record['airplaneRegID'] . "\" maxlength=\"16\" size=\"16\"/></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_airplaneColors\">Color:</label><input id=\"in_airplaneColors\" name=\"airplaneColors\" value=\"" . $record['airplaneColors'] . "\" maxlength=\"24\" size=\"24\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_boolean\"><input class=\"form_boolean\" id=\"in_currInspection\" type=\"checkbox\" name=\"currInspection\" " . boolChecked($record, 'currInspection') . ">Current annual or condition inspection</input></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_text\"><label for=\"in_engineMake\">Engine make:</label><input id=\"in_engineMake\" name=\"engineMake\" value=\"" . $record['engineMake'] . "\" maxlength=\"24\" size=\"18\"/></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_engineModel\">Model:</label><input id=\"in_engineModel\" name=\"engineModel\" value=\"" . $record['engineModel'] . "\" maxlength=\"24\" size=\"18\"/></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_engineHP\">Horsepower:</label><input id=\"in_engineHP\" name=\"engineHP\" value=\"" . $record['engineHP'] . "\" maxlength=\"6\" size=\"6\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td rowspan=\"2\" class=\"form_text\"><label for=\"in_insCompany\">Insurance company:</label><input id=\"in_insCompany\" name=\"insCompany\" value=\"" . $record['insCompany'] . "\" maxlength=\"24\" size=\"24\"/></td>\n";
    echo "<td class=\"form_boolean\"><input class=\"form_boolean\" id=\"in_liabilityAmt\" type=\"checkbox\" name=\"liabilityAmt\" " . boolChecked($record, 'liabilityAmt') . ">$1M min. liability</input></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_boolean\"><input class=\"form_boolean\" id=\"in_injuryAmt\" type=\"checkbox\" name=\"injuryAmt\" " . boolChecked($record, 'injuryAmt') . ">$100K min. personal injury</input></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_text\"><label for=\"in_insExpires\">Insurance expiration (mm/dd/yyyy):</label><input id=\"in_insExpires\" name=\"insExpires\" value=\"" . $record['insExpires'] . "\" maxlength=\"10\" size=\"10\"/></td>\n";

    echo "</tr>\n";
    echo "</tbody></table>\n";

    // Submission
    echo '<div class="regSubmit">' . "\n";
    echo '<input tabindex="1" class="submit" name="submit" type="submit" value="Save and Continue Registration"/>' . "\n";
    echo '<input tabindex="1" class="submit" name="save" type="submit" value="Save and Return"/>' . "\n";
    echo "</div>";

    // Pilot fields (all hidden)
    echo "<input type=\"hidden\" id=\"in_givenName\" name=\"givenName\" value=\"" . $record['givenName'] . "\" maxlength=\"72\" size=\"32\"/>\n";
    echo "<input type=\"hidden\" id=\"in_familyName\" name=\"familyName\" value=\"" . $record['familyName'] . "\" maxlength=\"72\" size=\"32\"/>\n";
    echo "<input type=\"hidden\" id=\"in_contactPhone\" name=\"contactPhone\" value=\"" . $record['contactPhone'] . "\" maxlength=\"16\" size=\"16\"/>\n";
    echo "<input type=\"hidden\" id=\"in_address\" name=\"address\" value=\"" . $record['address'] . "\" maxlength=\"72\" size=\"60\"/>\n";
    echo "<input type=\"hidden\" id=\"in_city\" name=\"city\" value=\"" . $record['city'] . "\" maxlength=\"24\" size=\"18\"/>\n";
    echo "<input type=\"hidden\" id=\"in_state\" name=\"state\" value=\"" . $record['state'] . "\" maxlength=\"24\" size=\"18\"/>\n";
    echo "<input type=\"hidden\" id=\"in_country\" name=\"country\" value=\"" . $record['country'] . "\" maxlength=\"24\" size=\"18\"/>\n";
    echo "<input type=\"hidden\" id=\"in_postalCode\" name=\"postalCode\" value=\"" . $record['postalCode'] . "\" maxlength=\"12\" size=\"12\"/>\n";
    echo "<input type=\"hidden\" id=\"in_compType\" name=\"compType\" value=\"" . $record['compType'] . "\"/>\n";
    

    echo "</form>\n";
}
?>
<?php
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
