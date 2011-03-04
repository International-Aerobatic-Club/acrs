<?php
set_include_path('./include'); 
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");
require_once ("data/encodePOST.inc");
require_once ("data/encodeHTML.inc");
require_once ('data/validMMDD.inc');
require_once ("useful.inc");
require_once ("redirect.inc");
require_once ('query/catQueries.inc');

// useful here
$catList = array (
    'primary',
    'sportsman',
    'intermediate',
    'advanced',
    'unlimited',
);

function classSelectionForm($record)
{
    echo "<form class=\"recordForm\" action=\"categoryWizard.php\" method=\"post\">\n";
    echo "<label for=\"in_class\">Contest will host airplane classes:</label><fieldset id=\"in_class\" legend=\"Class\">\n";
    echo "<input class=\"form_check\" type=\"checkbox\" id=\"in_class_power\" name=\"class_power\">power</input>\n";
    echo "<input class=\"form_check\" type=\"checkbox\" id=\"in_class_glider\" name=\"class_glider\">glider</input>\n";
    echo "</fieldset></td>\n";
    echo "<div class=\"submit\">\n";
    echo "<input class=\"submit\" name=\"classes\" type=\"submit\" value=\"next\"/>\n";
    echo "</div>";
    echo "</form>\n";
}

function categoryCheck($class, $name)
{
    echo "<input class=\"form_check\" type=\"checkbox\" id=\"in_category_" .
    $name . "\" name=\"category_" . $class . '_' . $name . "\">" .
    $name . "</input>\n";
}

function copyClassSelection($record)
{
    if (testChecked($record, 'class', 'power'))
        echo "<input type=\"hidden\" id=\"in_class_power\" name=\"class_power\" value=\"on\"/>\n";
    if (testChecked($record, 'class', 'glider'))
        echo "<input type=\"hidden\" id=\"in_class_glider\" name=\"class_glider\" value=\"on\"/>\n";
}

function powerSelectionForm($record)
{
    echo "<form class=\"recordForm\" action=\"categoryWizard.php\" method=\"post\">\n";
    echo "<label for=\"in_category\">";
    echo 'Contest will host <b>Power</b> categories:';
    echo "</label><fieldset id=\"in_category\" legend=\"Category\">\n";
    categoryCheck('power', 'primary');
    categoryCheck('power', 'sportsman');
    categoryCheck('power', 'intermediate');
    categoryCheck('power', 'advanced');
    categoryCheck('power', 'unlimited');
    echo "</fieldset></td>\n";
    echo "<label for=\"in_regFee\">Registration fee:</label>\n";
    echo "<input id=\"in_regFee\" name=\"power_regFee\" maxlength=\"4\" size=\"4\"/>\n";
    echo "<div class=\"submit\">\n";
    echo "<input class=\"submit\" name=\"power_categories\" type=\"submit\" value=\"next\"/>\n";
    echo "</div>";
    copyClassSelection($record);
    echo "</form>\n";
}

function gliderSelectionForm($record)
{
    echo "<form class=\"recordForm\" action=\"categoryWizard.php\" method=\"post\">\n";
    echo "<label for=\"in_category\">";
    echo 'Contest will host <b>Glider</b> categories:';
    echo "</label><fieldset id=\"in_category\" legend=\"Category\">\n";
    categoryCheck('glider', 'sportsman');
    categoryCheck('glider', 'intermediate');
    categoryCheck('glider', 'unlimited');
    echo "</fieldset></td>\n";
    echo "<label for=\"in_regFee\">Registration fee:</label>\n";
    echo "<input id=\"in_regFee\" name=\"glider_regFee\" value=\"" . inthtml($record['power_regFee'], 4) . "\"";
    echo " maxlength=\"4\" size=\"4\"/>\n";
    echo "<div class=\"submit\">\n";
    echo "<input class=\"submit\" name=\"glider_categories\"";
    echo " type=\"submit\" value=\"next\"/>\n";
    echo "</div>";
    copyClassSelection($record);
    // copy power selection
    if (testChecked($record, 'category_power', 'primary'))
        echo "<input type=\"hidden\" id=\"in_category_power_primary\" name=\"category_power_primary\" value=\"on\"/>\n";
    if (testChecked($record, 'category_power', 'sportsman'))
        echo "<input type=\"hidden\" id=\"in_category_power_sportsman\" name=\"category_power_sportsman\" value=\"on\"/>\n";
    if (testChecked($record, 'category_power', 'intermediate'))
        echo "<input type=\"hidden\" id=\"in_category_power_intermediate\" name=\"category_power_intermediate\" value=\"on\"/>\n";
    if (testChecked($record, 'category_power', 'advanced'))
        echo "<input type=\"hidden\" id=\"in_category_power_advanced\" name=\"category_power_advanced\" value=\"on\"/>\n";
    if (testChecked($record, 'category_power', 'unlimited'))
        echo "<input type=\"hidden\" id=\"in_category_power_unlimited\" name=\"category_power_unlimited\" value=\"on\"/>\n";
    if (testChecked($record, 'category_power', '4min'))
        echo "<input type=\"hidden\" id=\"in_category_power_4min\" name=\"category_power_4min\" value=\"on\"/>\n";
    echo "<input type=\"hidden\" id=\"in_power_regFee\" name=\"power_regFee\" value=\"" . inthtml($record['power_regFee'], 4) . "\"/>\n";
    echo "</form>\n";
}

function studentRegForm($record)
{
    echo "<form class=\"recordForm\" action=\"categoryWizard.php\" method=\"post\">\n";
    echo "<label for=\"in_class\">Student registrations accepted for categories:</label><fieldset id=\"in_class\" legend=\"Student categories\">\n";
    $power_cats = explode(',', $record['power_cats']);
    foreach ($power_cats as $cat)
    {
        echo "<input class=\"form_check\" type=\"checkbox\" id=\"in_student_power\"" . $cat . " name=\"student_power_" . $cat . "\">power " . $cat . "</input>\n";
    }
    $glider_cats = explode(',', $record['glider_cats']);
    foreach ($glider_cats as $cat)
    {
        echo "<input class=\"form_check\" type=\"checkbox\" id=\"in_student_glider\"" . $cat . " name=\"student_glider_" . $cat . "\">glider " . $cat . "</input>\n";
    }
    echo "</fieldset></td>\n";
    echo "<label for=\"in_regFee\">Registration fee:</label>";
    echo "<input id=\"in_regFee\" name=\"student_regFee\" maxlength=\"4\" size=\"4\"/>\n";
    echo "<div class=\"submit\">\n";
    echo "<input class=\"submit\" name=\"completeWizardry\" type=\"submit\" value=\"finish\"/>\n";
    echo "</div>";
    copyClassSelection($record);
    echo "<input type=\"hidden\" id=\"in_power_regFee\" name=\"power_regFee\" value=\"" . inthtml($record['power_regFee'], 4) . "\"/>\n";
    echo "<input type=\"hidden\" id=\"in_glider_regFee\" name=\"glider_regFee\" value=\"" . inthtml($record['glider_regFee'], 4) . "\"/>\n";
    echo "<input type=\"hidden\" id=\"in_power_cats\" name=\"power_cats\" value=\"" . strhtml($record['power_cats']) . "\"/>\n";
    echo "<input type=\"hidden\" id=\"in_glider_cats\" name=\"glider_cats\" value=\"" . strhtml($record['glider_cats']) . "\"/>\n";
    echo "</form>\n";
}

function initCat(& $category, $class, $cat)
{
    $category['ctstID'] = $_SESSION['ctstID'];
    $category['hasStudentReg'] = 'n';
    $category['hasTeamReg'] = 'n';
    $category['hasVoteJudge'] = 'n';
    $category['voteTeamOnly'] = 'n';
    $category['voteByRegion'] = 'n';
    $category['name'] = ucfirst($class) . ' ' . ucfirst($cat);
    $category['category'] = $cat;
    $category['class'] = $class;
}

function doSetupClass($db_conn, $record, $class)
{
    //debug("entering categoryWizard.doSetupClass for class, '".$class."'");
    //debugArr("categoryWizard.doSetupClass received record", $record);
    $fail = '';
    $catsArray = array ();
    $cat_arr = explode(',', $record[$class . '_cats']);
    //debugArr("categoryWizard.doSetupClass has categories", $cat_arr);
    if ($cat_arr !== false)
        foreach ($cat_arr as $cat)
        {
            $category = array ();
            $result = retrieveCatForCtstClassCat($db_conn, $category, $_SESSION['ctstID'], $class, $cat);
            if ($result != '')
            {
                initCat($category, $class, $cat);
            }
            $category['regAmt'] = $record[$class . '_regFee'];
            $catsArray[$cat] = $category;
        }
    global $catList;
    $sreg = selectionSet($record, 'student_' . $class, $catList);
    $cat_arr = explode(',', $sreg);
    if ($cat_arr !== false)
        foreach ($cat_arr as $cat)
        {
            $category = $catsArray[$cat];
            if (isset ($category))
            {
                $category['hasStudentReg'] = 'y';
                $category['studentRegAmt'] = $record['student_regFee'];
            }
        }
    foreach ($catsArray as $category)
    {
        $result = insertOrUpdateCategory($db_conn, $category, $category['catID']);
        if ($result != '')
            $fail .= '<li>' . $result . '</li>';
    }
    return $fail;
}

function doSetupCategories($db_conn, $record)
{
    $result = '';
    if ($record[class_power])
    {
      $result = doSetupClass($db_conn, $record, 'power');
    }
    if ($record[class_glider])
    {
      $result .= doSetupClass($db_conn, $record, 'glider');
    }
    return $result;
}

function afterClasses($record)
{
    if (isSet ($record['class_power']))
        powerSelectionForm($record);
    else
        if (isSet ($record['class_glider']))
            gliderSelectionForm($record);
        else
        {
            echo '<p class="error">Select at least one class</p>';
            classSelectionForm($record);
        }
}

/**
 * Aggregate the selected categories into a set for power and a set for glider.
 * Sets $record['powerCats'] and $record['gliderCats'].
 * Output the class selection form with an error if no categories selected,
 * else output the student registration form.
 */
function catAggregation($record)
{
    global $catList;
    $record['power_cats'] = selectionSet($record, 'category_power', $catList);
    $record['glider_cats'] = selectionSet($record, 'category_glider', $catList);
    if ($record['power_cats'] != '' || $record['glider_cats'] != '')
    {
        studentRegForm($record);
    } else
    {
        echo '<p class="error">Select at least one category for the contest</p>';
        classSelectionForm($record);
    }
}

function afterPower($record)
{
    // TODO validate power reg amt
    if (isSet ($record['class_glider']))
        gliderSelectionForm($record);
    else
        catAggregation($record);
}

function afterGlider($record)
{
    // TODO validate glider reg amt
    catAggregation($record);
}

function processForm($catInfo)
{
    $corrMsg = '';
    $userID = $_SESSION['userID'];
    $ctstID = $_SESSION['ctstID'];
    $db_conn = false;
    $fail = dbConnect($db_conn);
    if ($fail != '')
    {
        notifyError($fail, "catForm.php");
        $corrMsg = "<li>Internal: failed access to contest database</li>";
    } else
    {
        //debugArr('categoryWizard post data:', $catInfo);
        if (isset ($catInfo['completeWizardry']))
        {
            //TODO validate student reg amt
            $corrMsg = doSetupCategories($db_conn, $catInfo);
            if ($corrMsg == '')
                getNextPage('catTable.php');
        }
        startHead("Contest Categories");
        echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
        startContent();
        echo "<h1>Contest Category Wizard</h1>";
        verificationHeader("Contest official,");
        if ($corrMsg != '')
        {
            echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
        }
        if (isset ($catInfo['classes']))
        {
            afterClasses($catInfo);
        } else
            if (isset ($catInfo['power_categories']))
            {
                afterPower($catInfo);
            } else
                if (isset ($catInfo['glider_categories']))
                {
                    afterGlider($catInfo);
                } else
                {
                    classSelectionForm($catInfo);
                }
        echo '<div class="returnButton"><a href="catTable.php">Return without changes</a></div>';
        endContent();
        dbClose($db_conn);
    }
}

if (isContestAdmin())
{
    processForm($_POST);
} else
{
    getNextPage('index.html');
}
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
