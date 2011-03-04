<?php
set_include_path('./include');
require_once ("useful.inc");
require_once ("ui/siteLayout.inc");
require_once ("data/encodeHTML.inc");

function showData($value)
{
   if (is_array($value))
   {
      echo '<dl>';
      foreach ($value as $key => $value)
      {
         echo '<dt>'.$key.'</dt>';
         echo '<dd>';
         showData($value);
         echo '</dd>';
      }
      echo '</dl>';
   }
   else
   {
      echo strhtml($value);
   }
}

startHead("Checkbox Test");
startContent();
if (isset($_POST["send"]))
{
   echo '<p>Post content:';
   showData($_POST);
   echo '</p>';

   $list = implode (",", $_POST ["volunteer_judge"]);
   echo '<p>Imploded volunteer_judge:';
   showData($list);
   echo '</p>';
}
else
{
   ?>
<FORM method="post"><INPUT name="volunteer_judge[]" value="Any"
	type=checkbox> Any&nbsp;&nbsp; <INPUT name="volunteer_judge[]"
	value="Primary" type=checkbox> P&nbsp;&nbsp; <INPUT
	name="volunteer_judge[]" value="Sportsman" type=checkbox> S&nbsp;&nbsp;
<INPUT name="volunteer_judge[]" value="Intermediate" type=checkbox>
I&nbsp;&nbsp; <INPUT name="volunteer_judge[]" value="Advanced"
	type=checkbox> A&nbsp;&nbsp; <INPUT name="volunteer_judge[]"
	value="Unlimited" type=checkbox> U&nbsp;&nbsp; <INPUT name="send"
	type="submit" /></FORM>
   <?php
}
endContent();
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
