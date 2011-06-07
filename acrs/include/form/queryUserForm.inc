<?php
require_once("data/encodeHTML.inc");

/**
 Provides style settings and script for the form. Call from the header
 */
function queryUserFormHeader()
{
   echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
}

/**
 Creates a form for query a user
 This form collects contest information.
 $action is html sanitized url for the http post
 */
function queryUserForm($action)
{
   echo '<p>Provide any of the following:</p>';
   echo '<form id="regForm" class="regForm" action="'.$action.'" method="post">'."\n";
   echo "<p class=\"form_text\"><label for=\"in_email\">User email:</label><input id=\"in_email\" name=\"email\" maxlength=\"256\" size=\"32\"/></p>\n";
   echo "\n";
   echo "<p class=\"form_text\"><label for=\"in_givenName\">First Name:</label><input id=\"in_givenName\" name=\"givenName\" maxlength=\"72\" size=\"32\"/></p>\n";
   echo "<p class=\"form_text\"><label for=\"in_familyName\">Last Name:</label><input id=\"in_familyName\" name=\"familyName\" maxlength=\"72\" size=\"32\"/></p>\n";

   // Submission
   echo '<div class="regSubmit">'."\n";
   echo '<input class="submit" name="submit" type="submit" value="FindUser"/>'."\n";
   echo '</div>'."\n";
   echo '</form>';
}
?>
<?php
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
