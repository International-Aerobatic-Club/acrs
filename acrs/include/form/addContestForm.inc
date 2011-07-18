<?php
require_once("data/encodeHTML.inc");

/**
 Provides style settings and script for the form. Call from the header
 */
function addContestFormHeader()
{
   echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
}

/**
 Creates a form for adding a contest
 This form collects contest information.
 $record POST format contest data
 $action is html sanitized url for the http post
 */
function addContestForm($record, $action)
{
   echo '<form id="regForm" class="regForm" action="'.$action.'" method="post" onreset="checkDisplay()" onsubmit="enableAllForPost()">'."\n";
   echo '<table><tbody>'."\n";
   echo '<tr>'."\n";
   echo "<td colspan=\"2\" class=\"form_text\"><label for=\"in_name\">Contest name:</label><input id=\"in_name\" name=\"name\" value=\"".$record['name']."\" maxlength=\"72\" size=\"32\"/></td>\n";
   echo "</tr><tr>\n";
   echo "<td class=\"form_text\"><label for=\"in_location\">Location:</label><input id=\"in_location\" name=\"location\" value=\"".$record['location']."\" maxlength=\"72\" size=\"32\"/></td>\n";
   echo "<td class=\"form_text\"><label for=\"in_chapter\">Chapter:</label><input id=\"in_chapter\" name=\"chapter\" value=\"".$record['chapter']."\" maxlength=\"4\" size=\"4\"/></td>\n";
   echo "</tr><tr>\n";
   echo "<td class=\"form_text\"><label for=\"in_regYear\">Year (YYYY):</label><input id=\"in_regYear\" name=\"regYear\" value=\"".$record['regYear']."\" maxlength=\"4\" size=\"4\"/></td>\n";
   echo "<td/></tr><tr>\n";
   echo "<td class=\"form_text\"><label for=\"in_regOpen\">First date to register(MM/DD):</label><input id=\"in_regOpen\" name=\"regOpen\" value=\"".$record['regOpen']."\" maxlength=\"5\" size=\"5\"/></td>\n";
   echo "<td class=\"form_text\"><label for=\"in_regDeadline\">Last date to register(MM/DD):</label><input id=\"in_regDeadline\" name=\"regDeadline\" value=\"".$record['regDeadline']."\" maxlength=\"5\" size=\"5\"/></td>\n";
   echo "</tr><tr>\n";
   echo "<td class=\"form_text\"><label for=\"in_startDate\">First date of competition flying(MM/DD):</label><input id=\"in_startDate\" name=\"startDate\" value=\"".$record['startDate']."\" maxlength=\"5\" size=\"5\"/></td>\n";
   echo "<td class=\"form_text\"><label for=\"in_endDate\">Last date of competition flying(MM/DD):</label><input id=\"in_endDate\" name=\"endDate\" value=\"".$record['endDate']."\" maxlength=\"5\" size=\"5\"/></td>\n";
   echo "<td/></tr><tr>\n";
   echo "<td colspan=\"2\"  class=\"form_text\"><label for=\"in_homeURL\">Announcement Web address:</label><input id=\"in_homeURL\" name=\"homeURL\" value=\"".$record['homeURL']."\" maxlength=\"320\" size=\"32\"/></td>\n";
   echo "</tr><tr>\n";
   echo "<td colspan=\"2\"  class=\"form_text\"><label for=\"in_regEmail\">Registration email contact:</label><input id=\"in_regEmail\" name=\"regEmail\" value=\"".$record['regEmail']."\" maxlength=\"320\" size=\"32\"/></td>\n";
   echo "</tr><tr>\n";
   echo "<td colspan=\"2\"  class=\"form_text\"><label for=\"in_question\">Optional announcement, question for response or further information (text only, no tags):</label><textarea id=\"in_question\" name=\"question\" cols=\"78\" maxlength=\"4096\" rows=\"4\">".$record['question']."</textarea></td>\n";
   echo "</tr><tr>\n";
   echo "<td class=\"form_select\"><label for=\"in_hasPayPal\">Accepts PayPal registration payments?:</label><fieldset id=\"in_hasPayPal\" legend=\"Accepts PayPal registration payments?\">\n";
   echo "<input class=\"form_select\" id=\"in_hasPayPal_yes\" type=\"radio\" name=\"hasPayPal\" value=\"yes\" ".isSelected($record,'hasPayPal','yes').">yes</input>\n";
   echo "<input class=\"form_select\" id=\"in_hasPayPal_no\" type=\"radio\" name=\"hasPayPal\" value=\"no\" ".isSelected($record,'hasPayPal','no').">no</input>\n";
   echo "</fieldset></td>\n";
   echo "<td class=\"form_text\"><label for=\"in_payEmail\">PayPal account identifier:</label><input id=\"in_payEmail\" name=\"payEmail\" value=\"".$record['payEmail']."\" maxlength=\"320\" size=\"32\"/></td>\n";
   echo "</tr><tr>\n";
   echo "<td class=\"form_select\"><label for=\"in_hasVoteJudge\">Has judge voting?:</label><fieldset id=\"in_hasVoteJudge\" legend=\"Has judge voting?\">\n";
   echo "<input class=\"form_select\" id=\"in_hasVoteJudge_yes\" type=\"radio\" name=\"hasVoteJudge\" value=\"yes\" ".isSelected($record,'hasVoteJudge','yes').">yes</input>\n";
   echo "<input class=\"form_select\" id=\"in_hasVoteJudge_no\" type=\"radio\" name=\"hasVoteJudge\" value=\"no\" ".isSelected($record,'hasVoteJudge','no').">no</input>\n";
   echo "</fieldset></td>\n";
   echo "<td class=\"form_text\"><label for=\"in_voteEmail\">Voting email contact:</label><input id=\"in_voteEmail\" name=\"voteEmail\" value=\"".$record['voteEmail']."\" maxlength=\"320\" size=\"32\"/></td>\n";
   echo "</tr><tr>\n";
   echo "<td class=\"form_select\"><label for=\"in_reqPmtForVoteJudge\">Payment required before judge voting?:</label><fieldset id=\"in_reqPmtForVoteJudge\" legend=\"Payment required before judge voting?\">\n";
   echo "<input class=\"form_select\" id=\"in_reqPmtForVoteJudge_yes\" type=\"radio\" name=\"reqPmtForVoteJudge\" value=\"yes\" ".isSelected($record,'reqPmtForVoteJudge','yes').">yes</input>\n";
   echo "<input class=\"form_select\" id=\"in_reqPmtForVoteJudge_no\" type=\"radio\" name=\"reqPmtForVoteJudge\" value=\"no\" ".isSelected($record,'reqPmtForVoteJudge','no').">no</input>\n";
   echo "</fieldset></td>\n";
   echo "<td/>\n";
   echo "</tr><tr>\n";
   echo "<td class=\"form_select\"><label for=\"in_hasPracticeReg\">Accepts practice slot reservations?:</label><fieldset id=\"in_hasPracticeReg\" legend=\"Accepts practice slot reservations?\">\n";
   echo "<input class=\"form_select\" id=\"in_hasPracticeReg_yes\" type=\"radio\" name=\"hasPracticeReg\" value=\"yes\" ".isSelected($record,'hasPracticeReg','yes').">yes</input>\n";
   echo "<input class=\"form_select\" id=\"in_hasPracticeReg_no\" type=\"radio\" name=\"hasPracticeReg\" value=\"no\" ".isSelected($record,'hasPracticeReg','no').">no</input>\n";
   echo "</fieldset></td>\n";
   echo "<td class=\"form_text\"><label for=\"in_maxPracticeSlots\">Maximum slots per pilot:</label><input id=\"in_maxPracticeSlots\" name=\"maxPracticeSlots\" value=\"".$record['maxPracticeSlots']."\" maxlength=\"2\" size=\"2\"/></td>\n";
   echo "</tr><tr>\n";
   echo "<td class=\"form_select\"><label for=\"in_reqPmtForPracticeReg\">Payment required before slot reservation?:</label><fieldset id=\"in_reqPmtForPracticeReg\" legend=\"Payment required before slot reservation?\">\n";
   echo "<input class=\"form_select\" id=\"in_reqPmtForPracticeReg_yes\" type=\"radio\" name=\"reqPmtForPracticeReg\" value=\"yes\" ".isSelected($record,'reqPmtForPracticeReg','yes').">yes</input>\n";
   echo "<input class=\"form_select\" id=\"in_reqPmtForPracticeReg_no\" type=\"radio\" name=\"reqPmtForPracticeReg\" value=\"no\" ".isSelected($record,'reqPmtForPracticeReg','no').">no</input>\n";
   echo "</fieldset></td>\n";
   echo "<td/>\n";
   echo "</tr><tr>\n";

   // Submission
   echo '<td><div class="regSubmit">'."\n";
   if ($record['isEdit'])
   {
      echo '<input type="hidden" name="isEdit" value="true"/>';
      echo '<input tabindex="1" class="submit" name="save" type="submit" value="Save"/>'."\n";
   }
   else
   {
      echo '<input tabindex="1" class="submit" name="save" type="submit" value="Save and Continue"/>'."\n";
   }
   echo '</div></td>'."\n";
   echo '</tr></table></tbody>'."\n";
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
