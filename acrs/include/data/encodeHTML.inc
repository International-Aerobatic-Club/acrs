<?php

/**
 Routines to translate $_POST data format into value strings
 of HTML form
 */

/**
 Dates of the POST data format to HTML display
 Use it on non-HTML content dates written into HTML content.
 */
function datehtml($date, $fmt = '%m/%d/%Y')
{
   $d = strtotime($date);
   $dstr = strftime($fmt, $d);
   return htmlspecialchars($dstr, ENT_QUOTES);
}

/**
 Times of the POST data format to HTML display
 Use it on non-HTML content times written into HTML content.
 */
function timehtml($time, $fmt = '%H:%M')
{
   $d = strtotime($time);
   $dstr = strftime($fmt, $d);
   return htmlspecialchars($dstr, ENT_QUOTES);
}

/**
 Strings of the POST data format may already have escaped quotes.
 That depends on the magic_quotes_gpc value.
 Assume that the php.ini settings disable the magic.
 All values in POST data will have unescaped quotes and special characters.
 This wraps htmlspecialchars($str, ENT_QUOTES);
 Use it on non-HTML content strings written into HTML content.
 */
function strhtml($str)
{
   return htmlspecialchars($str, ENT_QUOTES);
}

/**
 This crops to length and strips non-digits
 Use it on non-HTML integer strings written into HTML content.
 */
function inthtml($str, $maxlen = 0)
{
   if ($maxlen > 0)
   {
      $strint = substr($str, 0, $maxlen);
   }
   else
   {
      $strint = $str;
   }
   $strint = preg_replace("/[^0-9]/", "", $strint);
   return $strint;
}

/**
 Integers and other numbers are already strings in the POST data.
 Use strhtml($intStr) just in case the user entered special characters.
 Clean-up the numeric data in the POST-to-database routines.
 Check the numeric data in the validation routines.
 */

/**
 POST format has value of the selected button for radio buttons
 post: the post data
 name: the name of the radio button
 value: the value of the radio button
 attribute: the attribute to set ="on", defaults to "checked" for radio and checkbox;
 provide "selected" for options.
 return the string "checked=\"on\"" if the given value matches the
 value for the name entry in the post data; otherwise, return an empty string.
 make the matching case-insensitive.
 Example usage:
 echo "<input class=\"check\" type=\"radio\" name=\"competitionLevel\"".
 " value=\"Primary\"".isSelected($_POST,"competitionLevel","Primary").
 ">primary</input>\n";
 */
function isSelected($post, $name, $value, $attribute = 'checked')
{
   $select = "";
   if (isSet ($post[$name]))
   {
      $setValue = trim(strtolower($post[$name]));
      $testValue = trim(strtolower($value));
      if (strcmp($setValue, $testValue) == 0)
      {
         $select = $attribute . '="on"';
      }
   }
   return $select;
}

/**
 POST format has the name of the input check box and keyword "on"
 for checked boxes.  The data omits values for unchecked boxes.
 post: the post data
 name: the name of the check box
 value: the value of the check box
 return the string "checked=\"on\"" if post["name.value"] is set
 with value, "on"; otherwise, return an empty string.
 Example usage:
 echo '<input class="check" name="recAero.rec" type="checkbox"'.
 isChecked($_POST,"recAero","rec").
 '>recreational aerobatics</input>'."\n";
 */
function isChecked($post, $name, $value)
{
   $check = "";
   $fieldName = $name . '_' . $value;
   if (isSet ($post[$fieldName]))
   {
      $setValue = trim(strtolower($post[$fieldName]));
      if (strcmp($setValue, "on") == 0)
      {
         $check = "checked=\"on\"";
      }
   }
   return $check;
}

/**
 POST format has the name of the input check box and keyword "on"
 for checked boxes.  The data omits values for unchecked boxes.
 post: the post data
 name: the name of the check box
 value: the value of the check box
 return true if post["name_value"] is set
 with value, "on"; otherwise, return false.
 Example usage:
 if (testChecked($_POST,"recAero","rec")) ...
 returns true if $_POST['recAero_rec'] has value 'on'
 */
function testChecked($post, $name, $value)
{
   $isChecked = false;
   $fieldName = $name . '_' . $value;
   if (isSet ($post[$fieldName]))
   {
      $setValue = trim(strtolower($post[$fieldName]));
      if (strcmp($setValue, 'on') == 0)
      {
         $isChecked = true;
      }
   }
   return $isChecked;
}

/**
 POST format has the name of the input check box and keyword "on"
 for checked boxes.  The data omits values for unchecked boxes.
 post: the post data
 field: the name of the boolean data field
 return the string "yes" if post["field"] is set
 with value, "on"; otherwise, return "no".
 Example usage:
 echo yesOrNo($_POST,"doesFly");
 */
function yesOrNo($post, $field)
{
   return testBool($post, $field) ? 'yes' : 'no';
}

/**
 POST format has the name of the input check box and keyword "on"
 for checked boxes.  The data omits values for unchecked boxes.
 post: the post data
 field: the name of the boolean data field
 return the string "true" if post["field"] is set
 with value, "on"; otherwise, return "false".
 Example usage:
 echo tfBool($_POST,"doesFly");
 */
function tfBool($post, $field)
{
   return testBool($post, $field) ? 'true' : 'false';
}

/**
 POST format has the name of the input check box and keyword "on"
 for checked boxes.  The data omits values for unchecked boxes.
 post: the post data
 field: the name of the boolean data field
 return the string "checked=\"on\"" if post["field"] is set
 with value, "on"; otherwise, return an empty string.
 Example usage:
 echo '<input class="check" name="doesFly" type="checkbox"'.
 isChecked($_POST,"doesFly").
 '>is a pilot</input>'."\n";
 */
function boolChecked($post, $field)
{
   return testBool($post, $field) ? 'checked="on"' : '';
}

/**
 POST format has the name of the input check box and keyword "on"
 for checked boxes.  The data omits values for unchecked boxes.
 post: the post data
 field: the name of the boolean data field
 return the boolean true if post["field"] is set
 with value, "on"; otherwise, return false.
 Example usage:
 echo testBool($_POST,"doesFly");
 */
function testBool($post, $field)
{
   $bool = false;
   if (isSet ($post[$field]))
   {
      $setValue = trim(strtolower($post[$field]));
      if (strcmp($setValue, 'on') == 0 || strcmp($setValue, 'yes') == 0 || strcmp($setValue, 'y') == 0)
      {
         $bool = true;
      }
   }
   return $bool;
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
