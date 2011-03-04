<?php
require_once ("data/encodeHTML.inc");

/**
Provides style settings and script for the form. Call from the header
*/
function volunteerFormHeader()
{
?>
<link href="regform.css" type="text/css" rel="stylesheet"/>
<script type="text/javascript" src="volform.js"></script>
<?php


}

function isCompCat($volunteer, $catID)
{
    return ($volunteer['compCatID'] == $catID);
}

function roleChecked($volunteer, $catID, $role)
{
    return isChecked($volunteer, 'volunteer_' . $catID, $role);
}

/**
 * Write a cell in the volunteer selection grid
 * $volunteer contains current volunteer selections and category descriptions.
 * $role is the set value of the volunteer column in the volunteer table
 * $catName name of the category
 * $catID is the id of the category
 */
function volunteerTableCell($volunteer, $role, $catName, $catID, $checkCall)
{
    echo "<td class='volunteer'>\n";
    echo "<input class='volunteer'\n";
    echo " id='id_" . $catID . "_" . $role . "'\n";
    echo " type='checkbox' name='volunteer_" . $catID . "_" . $role . "'\n";
    echo " onclick='".$checkCall."'\n";
    echo " " . roleChecked($volunteer, $catID, $role) . "\n";
    echo " >" . $catName . "</input></td>\n";
}

/**
 * Write the volunteer checkbox selection grid
 * $volunteer contains current volunteer selections and category descriptions.
 * $roles is an associative array of volunteer column value to role description.
 */
function volunteerTable($volunteer, $roles)
{
    $catList = explode('::', $volunteer['compCatIDList']);
    $nameList = explode('::', $volunteer['compCatNameList']);
    $compCat = array_combine($catList, $nameList);
    echo "<table class='volunteer'>\n";
    echo "<thead class='volunteer'>" .
    "<tr class='volunteer'>\n";
    echo "<th class='volunteerCorner'/>\n";
    echo "<th class='volunteerCat'>Any category</th>\n";
    foreach ($compCat as $catID => $catName)
    {
        if (!isCompCat($volunteer, $catID))
        {
            echo "<th class='volunteerCat'>" . $catName . "</th>\n";
        }
    }
    echo "</tr></thead>\n";
    echo "<tbody>\n";
    echo "<tr>\n";
    echo "<th class='volunteerRole'>Work any role</th>\n";
    volunteerTableCell($volunteer, 'any', 'Any Category', 'any', 'checkTable(this)');
    foreach ($compCat as $catID => $catName)
    {
        if (!isCompCat($volunteer, $catID))
        {
            volunteerTableCell($volunteer, 'any', $catName, $catID,'checkColumn(this)');
        }
    }
    echo "</tr>\n";
    foreach ($roles as $role => $description)
    {
        echo "<tr>\n";
        echo "<th class='volunteerRole'>" . $description . "</th>\n";
        volunteerTableCell($volunteer, $role, 'Any Category', 'any', 'checkRow(this)');
        foreach ($compCat as $catID => $catName)
        {
            if (!isCompCat($volunteer, $catID))
            {
                volunteerTableCell($volunteer, $role, $catName, $catID, 'uncheckRowColumn(this)');
            }
        }
    }
    echo "</tbody></table>\n";
}

/**
Creates a form for editing registrant data.
This form collects contact information and registration type.
$volunteer POST format registration data
$action is html sanitized url for the http post
*/
function volunteerForm($volunteer, $action, $roles)
{
    echo "<form class='volunteer' action='" . $action . "' method='post'>\n";
    volunteerTable($volunteer, $roles);
    echo "<input type='hidden' name='givenName' value='" . $volunteer['givenName'] . "'/>\n";
    echo "<input type='hidden' name='familyName' value='" . $volunteer['familyName'] . "'/>\n";

    echo "<div class='volunteerJudge'>\n";
    echo "<label for=\"in_judgeQualification\">Current line judge qualification:</label>";
    echo "<fieldset id=\"in_judgeQualification\" legend=\"Current scoring judge qualification\">\n";
    echo "<input class=\"form_select\" id=\"in_judgeQualification_none\" type=\"radio\" name=\"judgeQualification\" value=\"none\" " . isSelected($volunteer, 'judgeQualification', 'none') . ">none</input>\n";
    echo "<input class=\"form_select\" id=\"in_judgeQualification_regional\" type=\"radio\" name=\"judgeQualification\" value=\"regional\" " . isSelected($volunteer, 'judgeQualification', 'regional') . ">regional</input>\n";
    echo "<input class=\"form_select\" id=\"in_judgeQualification_national\" type=\"radio\" name=\"judgeQualification\" value=\"national\" " . isSelected($volunteer, 'judgeQualification', 'national') . ">national</input>\n";
    echo "</fieldset>\n";
    echo "</div>\n";
    
    echo "<input type=\"hidden\" name=\"compCatID\" value=\"".$volunteer['compCatID']."\"/>\n";
    echo "<input type=\"hidden\" name=\"compCatName\" value=\"".$volunteer['compCatName']."\"/>\n";
    echo "<input type=\"hidden\" name=\"compCatIDList\" value=\"".$volunteer['compCatIDList']."\"/>\n";
    echo "<input type=\"hidden\" name=\"compCatNameList\" value=\"".$volunteer['compCatNameList']."\"/>\n";

    echo "<div class='volunteerSubmit'>\n";
    if ($volunteer['compType'] == 'competitor')
    {
        echo "<input class='submit' name='submit' type='submit' value='Save and Continue Registration'/>\n";
    }
    echo "<input class='submit' name='save' type='submit' value='Save and Return'/>\n";
    echo "</div>\n";

    echo "</form>\n";
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
