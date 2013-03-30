<?php
//--------------------------------------------------------------------
//
// Author:      Peter M. Jensen
// Date:        23-May-2005
// Description:
//
//--------------------------------------------------------------------

require_once ('cform2.inc');

define ("FONT",        "Helvetica");
define ("FONT_BOLD",   "Helvetica-Bold");
define ("FONT_ITALIC", "Helvetica-Oblique");
define ("FILL_FONT",   "Courier");
define ("BOX_SIZE",    8);

function Value ($fields, $key)
{
    if (isset ($fields [$key])) {
        return $fields [$key];
    }
    else {
        return "";
    }
}

function Make_Entry_Template (&$form, $page)
{
    // Logo

    $form->CForm_Add_Image ($page, 9, 6, 20, "logos/logo.png");

    // Header

    $form->CForm_Add_Text_Line   ($page, 45, 8, FONT_BOLD, 12, "INTERNATIONAL AEROBATIC CLUB");
    $form->CForm_Add_Text_Line   ($page, 51, 11, FONT_BOLD, 12, "Official Contest Entry Form");

    $form->CForm_Add_Double_Line ($page, 8, 27, 128);

    // Contest Information
    
    $y = 30;

    $form->CForm_Add_Text_Line   ($page, 8,   $y, FONT_BOLD, 10, "CONTEST:");
    $form->CForm_Add_Text_Line   ($page, 22,  $y, FONT, 10, "Name:");
    $form->CForm_Add_UnderLine   ($page, 34,  $y, 88, 10);
    $form->CForm_Add_Text_Line   ($page, 92,  $y, FONT, 10, "Date:");
    $form->CForm_Add_UnderLine   ($page, 100, $y, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 5, FONT, 10, "Location:");
    $form->CForm_Add_UnderLine   ($page, 34,  $y + 5, 88, 10);
    $form->CForm_Add_Text_Line   ($page, 92,  $y + 5, FONT, 10, "Type:");
    $form->CForm_Add_Checkbox    ($page, 100, $y + 5, 10);
    $form->CForm_Add_Text_Line   ($page, 103, $y + 5, FONT, 10, "Power");
    $form->CForm_Add_Checkbox    ($page, 112, $y + 5, 10);
    $form->CForm_Add_Text_Line   ($page, 115, $y + 5, FONT, 10, "Glider");
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 10, FONT, 10, "Contest Category:");
    $form->CForm_Add_Checkbox    ($page, 43,  $y + 10, 10);
    $form->CForm_Add_Text_Line   ($page, 46,  $y + 10, FONT, 10, "Primary");
    $form->CForm_Add_Checkbox    ($page, 57,  $y + 10, 10);
    $form->CForm_Add_Text_Line   ($page, 60,  $y + 10, FONT, 10, "Sportsman");
    $form->CForm_Add_Checkbox    ($page, 75,  $y + 10, 10);
    $form->CForm_Add_Text_Line   ($page, 78,  $y + 10, FONT, 10, "Intermediate");
    $form->CForm_Add_Checkbox    ($page, 95,  $y + 10, 10);
    $form->CForm_Add_Text_Line   ($page, 98,  $y + 10, FONT, 10, "Advanced");
    $form->CForm_Add_Checkbox    ($page, 112, $y + 10, 10);
    $form->CForm_Add_Text_Line   ($page, 115, $y + 10, FONT, 10, "Unlimited");
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 15, FONT, 10, "Patch Category:");
    $form->CForm_Add_Checkbox    ($page, 43,  $y + 15, 10);
    $form->CForm_Add_Text_Line   ($page, 46,  $y + 15, FONT, 10, "Primary");
    $form->CForm_Add_Checkbox    ($page, 57,  $y + 15, 10);
    $form->CForm_Add_Text_Line   ($page, 60,  $y + 15, FONT, 10, "Sportsman");
    $form->CForm_Add_Checkbox    ($page, 75,  $y + 15, 10);
    $form->CForm_Add_Text_Line   ($page, 78,  $y + 15, FONT, 10, "Intermediate");
    $form->CForm_Add_Checkbox    ($page, 95,  $y + 15, 10);
    $form->CForm_Add_Text_Line   ($page, 98,  $y + 15, FONT, 10, "Advanced");
    $form->CForm_Add_Checkbox    ($page, 112, $y + 15, 10);
    $form->CForm_Add_Text_Line   ($page, 115, $y + 15, FONT, 10, "Unlimited");
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 20, FONT, 10, "First-Time Competitor:");
    $form->CForm_Add_Checkbox    ($page, 47,  $y + 20, 10);
    $form->CForm_Add_Text_Line   ($page, 51,  $y + 20, FONT, 10, "(mark if, first time flying in chosen contest category)");

    $form->CForm_Add_Double_Line ($page, 8,   $y + 25, 128);

    // Pilot Information
    
    $y = $y + 28;

    $form->CForm_Add_Text_Line   ($page, 8,   $y, FONT_BOLD, 10, "PILOT:");
    $form->CForm_Add_Text_Line   ($page, 22,  $y, FONT, 10, "Name:");
    $form->CForm_Add_UnderLine   ($page, 30,  $y, 80, 10);
    $form->CForm_Add_Text_Line   ($page, 86,  $y, FONT, 10, "IAC Number:");
    $form->CForm_Add_UnderLine   ($page, 100, $y, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 5, FONT, 10, "Address:");
    $form->CForm_Add_UnderLine   ($page, 32,  $y + 5, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 32,  $y + 8, FONT, 8, "Street");
    $form->CForm_Add_Text_Line   ($page, 72,  $y + 8, FONT, 8, "City");
    $form->CForm_Add_Text_Line   ($page, 104, $y + 8, FONT, 8, "State/Zip");
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 12, FONT, 10, "Country:");
    $form->CForm_Add_UnderLine   ($page, 32,  $y + 12, 75, 10);
    $form->CForm_Add_Text_Line   ($page, 81,  $y + 12, FONT, 10, "U.S. Citizen:");
    $form->CForm_Add_Checkbox    ($page, 96,  $y + 12, 10);
    $form->CForm_Add_Text_Line   ($page, 99,  $y + 12, FONT, 10, "Yes");
    $form->CForm_Add_Checkbox    ($page, 106,  $y + 12, 10);
    $form->CForm_Add_Text_Line   ($page, 109,  $y + 12, FONT, 10, "No");

    $y = $y + 5;

    $form->CForm_Add_Text_Line   ($page, 22,  $y + 12, FONT, 10, "E-mail Address:");
    $form->CForm_Add_UnderLine   ($page, 39,  $y + 12, 80, 10);
    $form->CForm_Add_Text_Line   ($page, 86,  $y + 12, FONT, 10, "Birth Date:");
    $form->CForm_Add_UnderLine   ($page, 98,  $y + 12, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 17, FONT, 10, "Phone:");
    $form->CForm_Add_UnderLine   ($page, 30,  $y + 17, 66, 10);
    $form->CForm_Add_Text_Line   ($page, 72,  $y + 17, FONT, 10, "Cell Phone:");
    $form->CForm_Add_UnderLine   ($page, 85,  $y + 17, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 22, FONT, 10, "IAC Chapter:");
    $form->CForm_Add_UnderLine   ($page, 36,  $y + 22, 72, 10);
    $form->CForm_Add_Text_Line   ($page, 76,  $y + 22, FONT, 10, "FAI Sporting License:");
    $form->CForm_Add_UnderLine   ($page, 99,  $y + 22, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 27, FONT, 10, "License Type:");
    $form->CForm_Add_UnderLine   ($page, 37,  $y + 27, 72, 10);
    $form->CForm_Add_Text_Line   ($page, 74,  $y + 27, FONT, 10, "License Number:");
    $form->CForm_Add_UnderLine   ($page, 92,  $y + 27, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 32, FONT, 10, "Emergency Contact:");
    $form->CForm_Add_Underline   ($page, 45,  $y + 32, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 45,  $y + 35, FONT, 8, "Name");
    $form->CForm_Add_Text_Line   ($page, 86,  $y + 35, FONT, 8, "Phone number(s)");
    $form->CForm_Add_Double_Line ($page, 8,   $y + 39, 128);

    // Owner Information
    
    $y = $y + 42;

    $form->CForm_Add_Text_Line   ($page, 8,   $y, FONT_BOLD, 10, "OWNER:");
    $form->CForm_Add_Text_Line   ($page, 22,  $y, FONT, 10, "Name:");
    $form->CForm_Add_UnderLine   ($page, 30,  $y, 78, 10);
    $form->CForm_Add_Text_Line   ($page, 82,  $y, FONT, 10, "Phone Number:");
    $form->CForm_Add_UnderLine   ($page, 99,  $y, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 5, FONT, 10, "Address:");
    $form->CForm_Add_UnderLine   ($page, 32,  $y + 5, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 32,  $y + 8, FONT, 8, "Street");
    $form->CForm_Add_Text_Line   ($page, 72,  $y + 8, FONT, 8, "City");
    $form->CForm_Add_Text_Line   ($page, 104, $y + 8, FONT, 8, "State/Zip");
    $form->CForm_Add_Double_Line ($page, 8,   $y + 12, 128);

    // Aircraft Information

    $y = $y + 15;
    
    $form->CForm_Add_Text_Line   ($page, 8,   $y, FONT_BOLD, 10, "AIRCRAFT:");
    $form->CForm_Add_Text_Line   ($page, 22,  $y, FONT, 10, "Make/Model:");
    $form->CForm_Add_UnderLine   ($page, 36,  $y, 82, 10);
    $form->CForm_Add_Text_Line   ($page, 86,  $y, FONT, 10, '"N" Number:');
    $form->CForm_Add_UnderLine   ($page, 100, $y, 126, 10);
    $form->CForm_Add_Checkbox    ($page, 23,  $y + 5, 10);
    $form->CForm_Add_Text_Line   ($page, 26,  $y + 5, FONT, 10, "Grass Roots Qualified");
    $form->CForm_Add_Text_Line   ($page, 68,  $y + 5, FONT, 10, 'Type of Airworthiness:');
    $form->CForm_Add_Checkbox    ($page, 92,  $y + 5, 10);
    $form->CForm_Add_Text_Line   ($page, 95,  $y + 5, FONT, 10, 'Experimental');
    $form->CForm_Add_Checkbox    ($page, 113, $y + 5, 10);
    $form->CForm_Add_Text_Line   ($page, 116, $y + 5, FONT, 10, 'Acrobatic');
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 10, FONT, 10, "Engine Make/Model/Horsepower:");
    $form->CForm_Add_UnderLine   ($page, 56,  $y + 10, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  $y + 15, FONT, 10, "Insurance Information:");
    $form->CForm_Add_UnderLine   ($page, 45,  $y + 15, 126, 10);
    $form->CForm_Add_Text_Line   ($page, 46,  $y + 18, FONT, 8, "Company");
    $form->CForm_Add_Text_Line   ($page, 80,  $y + 18, FONT, 8, "Type");
    $form->CForm_Add_Text_Line   ($page, 103, $y + 18, FONT, 8, "Amount");
    $form->CForm_Add_Double_Line ($page, 8,   $y + 22, 128);

    // Legalese
    $page = $page + 1;
    
    $y = 13;

    $form->CForm_Add_Text_Lines ($page, 8, $y, 128, FONT, 10,
       "The undersigned owner and pilot hereby for the entry in the aforementioned contest and, if permitted to participate " .
       "in said contest, agree to the following terms and conditions:");

    $form->CForm_Add_Text_Lines ($page, 8,  $y + 8, 11,  FONT, 10, "1.");
    $form->CForm_Add_Text_Lines ($page, 12, $y + 8, 128, FONT, 10, "That all statements on this application are true;");

    $form->CForm_Add_Text_Lines ($page, 8,  $y + 13, 11,  FONT, 10, "2.");
    $form->CForm_Add_Text_Lines ($page, 12, $y + 13, 128, FONT, 10,
        "That the owner, pilot and aforementioned aircraft currently comply and will continue to comply throughout this contest " .
        "with all applicable Federal Aviation Regulations and all IAC rules and regulations;");

    $form->CForm_Add_Text_Lines ($page, 8,  $y + 20, 11,  FONT, 10, "3.");
    $form->CForm_Add_Text_Lines ($page, 12, $y + 20, 128, FONT, 10,
        "That participation in this contest is not authorized until acceptance is evidenced below by the contest director and the " .
        "entry fee has been paid;");

    $form->CForm_Add_Text_Lines ($page, 8,  $y + 27, 11,  FONT, 10, "4.");
    $form->CForm_Add_Text_Lines ($page, 12, $y + 27, 128, FONT, 10,
        "That all the requirements of the various contest committees will be immediately compiled with upon request, including " .
        "technical inspections and attendance at all pilot briefings;");

    $y = $y + 34;
    
    $form->CForm_Add_Text_Lines ($page, 8,  $y,  11, FONT, 10, "5.");
    $form->CForm_Add_Text_Lines ($page, 12, $y, 128, FONT, 10,
        "That the undersigned assign and transfer any and all rights to photograph, tape, film, or in any way reproduce the " .
        "participation of owner, pilot and/or aircraft in this contest to the IAC, its agents, successors, or assigns;");

    $y = $y + 7;
    
    $form->CForm_Add_Text_Lines ($page, 8,  $y, 11,  FONT, 10, "6.");
    $form->CForm_Add_Text_Lines ($page, 12, $y, 128, FONT, 10,
        "That the undersigned, for themselves, their heirs, administrators, and executors, hereby accept full and complete " .
        "responsibility for and agree to release, indemnify, and hold the IAC, contest officials and sponsors, airport " .
        "owners and operators and any agents thereof harmless from any and all damages or liability whether caused by or " .
        "incurred by the undersigned as a result of their negligence, acts or omissions during participation in this contest;");

    $y = $y + 2;

    $form->CForm_Add_Text_Lines ($page, 8,  $y + 11, 11,  FONT, 10, "7.");
    $form->CForm_Add_Text_Lines ($page, 12, $y + 11, 128, FONT, 10,
        "That the contest director may at any time and for any reason terminate participation of the owner, pilot, or aircraft " .
        "in this contest, and neither the director nor the IAC shall incur any liability or further duty to the undersigned as " .
        "a result thereof.");

    $y = $y + 2;

    $form->CForm_Add_Text_Lines ($page, 8,  $y + 19, 11, FONT, 10, "8.");
    $form->CForm_Add_Text_Lines ($page, 12, $y + 19, 128, FONT, 10,
        "The International Aerobatic Club, Inc. as well as its chapters, officers, directors, employees, agents, contractors " .
        "and servants (all hereinafter referred to as \"IAC\"), are involved in varying degrees with the presentation of this " .
        "contest.  The IAC, HOWEVER, HAS ABSOLUTELY NO CONTROL NOR RESPONSIBILITY FOR ANY FLIGHT OR FLIGHT RELATED ACTIVITY " .
        "RESULTING FROM ANY INDIVIDUAL'S ATTENDANCE AT OR PARTICIPATION IN ANY PHASE OF SAID CONTEST.  ALL FLIGHT OR FLIGHT " .
        "RELATED ACTIVITY IS REGULATED BY THE RULES AND REGULATIONS OF THE FEDERAL AVIATION ADMINISTRATION and/or FIELD " .
        "PERSONNEL, INCLUDING BUT IN NO WAY LIMITED TO APPROACH, LANDING, TAXIING, PARKING, TAKEOFF, DEPARTURE, AND " .
        "PARTICIPATION IN SAID CONTEST.  ALL PILOTS OPERATE THEIR AIRCRAFT AS PILOT IN COMMAND, AT THEIR OWN RISK AND EACH " .
        "SUCH PILOT AS A CONDITION AND IN CONSIDERATION OF HIS OR HER ENGAGING IN SAID FLIGHT OR FLIGHT RELATED ACTIVITY THIS " .
        "CONTEST, FOR HIMSELF, HIS HEIRS AND ADMINISTRATORS, RELEASE AND AGREES TO INDEMNIFY AND HOLD THE IAC HARMLESS FROM " .
        "ANY AND ALL EXPENSES, DAMAGES, CAUSE OF ACTION, LOSSES, AND LIABILITY WHATSOEVER, WHETHER TO SAID PILOT OR TO " .
        "ANY PASSENGERS OR OTHER THIRD PARTIES.");

    $y = $y + 9;

    $form->CForm_Add_Text_Lines ($page, 8,  $y + 47, 11,  FONT, 10, "9.");
    $form->CForm_Add_Text_Lines ($page, 12, $y + 47, 128, FONT, 10,
        "The permitting of participation by contestant ((owner and/or pilot) in no way shall be an approval or certification " .
        "of the structural or mechanical ability of any aircraft or the competency of any pilot to perform the maneuvers " .
        "necessary to participate in said contest.");

    // Pilot Signature

    $y = $y + 7;

    $form->CForm_Add_Line      ($page, 8,  $y + 56, 72);
    $form->CForm_Add_Text_Line ($page, 8,  $y + 57, FONT, 10, "Pilot's Signature");
    $form->CForm_Add_Line      ($page, 80, $y + 56, 126);
    $form->CForm_Add_Text_Line ($page, 80, $y + 57, FONT, 10, "Date");

    // Owner Signature

    $form->CForm_Add_Line      ($page, 8,  $y + 64, 72);
    $form->CForm_Add_Text_Line ($page, 8,  $y + 65, FONT, 10, "Owner's Signature");
    $form->CForm_Add_Line      ($page, 80, $y + 64, 126);
    $form->CForm_Add_Text_Line ($page, 80, $y + 65, FONT, 10, "Date");

    $form->CForm_Add_Text_Line ($page, 8, $y + 70, FONT_BOLD, 10, "ACCEPTED FOR PARTICIPATION:");

    // CD Signature

    $form->CForm_Add_Line      ($page, 8, $y + 78, 72);
    $form->CForm_Add_Text_Line ($page, 8, $y + 79, FONT, 10, "Contest Director's Signature");
    $form->CForm_Add_Line      ($page, 80, $y + 78, 126);
    $form->CForm_Add_Text_Line ($page, 80, $y + 79, FONT, 10, "Date");
    
    // Document info
    
    $form->CForm_Add_Text_Line ($page, 114, 170, FONT, 6, "Tab 3 - Form 01");
}

function Make_Tech_Template (&$form, $page)
{
    // Logo

    $form->CForm_Add_Image ($page, 8, 6, 20, "logos/logo.png");

    // Header

    $form->CForm_Add_Text_Line   ($page, 50, 12, FONT_BOLD, 12, "INTERNATIONAL AEROBATIC CLUB");
    $form->CForm_Add_Text_Line   ($page, 53, 15, FONT, 12, "Official Technical Inspection Form");

    $form->CForm_Add_Line($page, 8, 22, 124);
    $form->CForm_Add_Text_Lines  ($page, 8, 23, 125, FONT, 9,
       "The following checklist is to be used for conducting technical " .
       "inspections at IAC Sonctioned events and meets the entrance " .
       "requirements for contest or " .
       "rally participation. NOTE: The Technical Inspection comprises a " .
       "thorough \"walk around\" and is not a condition inspection. Proof " .
       "of a current Annual or Condition Inspection is considered " .
       "adequate evidence of compliance with all applicable regulatory " .
       "inspections, AD's and Service Bulletins. Removal of cowlings, " .
       "inspection plates, etc., will not be required unless there is " .
       "obvious evidence of an underlying problem. Excessive or unfair " .
       "inspections may be the subject of a Protest.");
    $form->CForm_Add_Line ($page, 8, 36, 124);

    // Contest Information

    $form->CForm_Add_Text_Line   ($page, 8,   38, FONT_BOLD, 10, "CONTEST:");
    $form->CForm_Add_Text_Line   ($page, 22,  38, FONT, 10, "Name:");
    $form->CForm_Add_UnderLine   ($page, 34,  38, 123, 10);
    $form->CForm_Add_Text_Line   ($page, 86,  43, FONT, 10, "Date:");
    $form->CForm_Add_UnderLine   ($page, 94,  43, 123, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  43, FONT, 10, "Location:");
    $form->CForm_Add_UnderLine   ($page, 34,  43, 80, 10);
    $form->CForm_Add_Line ($page, 8, 47, 124);

    // Pilot Information
    
    $y = 49;

    $form->CForm_Add_Text_Line   ($page, 8,   $y, FONT_BOLD, 10, "PILOT:");
    $form->CForm_Add_Text_Line   ($page, 22,  $y, FONT, 10, "Name:");
    $form->CForm_Add_UnderLine   ($page, 30,  $y, 68, 10);
    $form->CForm_Add_Text_Line   ($page, 72,  $y, FONT, 10, "IAC #:");
    $form->CForm_Add_UnderLine   ($page, 80,  $y, 96, 10);
    $form->CForm_Add_Text_Line   ($page, 98,  $y, FONT, 10, "Please Initial:");
    $form->CForm_Add_UnderLine   ($page, 112, $y, 123, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  $y+5, FONT, 10, "Type:");
    $form->CForm_Add_Checkbox    ($page, 34,  $y+5, 10);
    $form->CForm_Add_Text_Line   ($page, 37,  $y+5, FONT, 10, "Power");
    $form->CForm_Add_Checkbox    ($page, 50,  $y+5, 10);
    $form->CForm_Add_Text_Line   ($page, 53,  $y+5, FONT, 10, "Glider");
    $form->CForm_Add_Text_Line   ($page, 22,  $y+10, FONT, 10, "Category:");
    $form->CForm_Add_Checkbox    ($page, 34,  $y+10, 10);
    $form->CForm_Add_Text_Line   ($page, 37,  $y+10, FONT, 10, "Primary");
    $form->CForm_Add_Checkbox    ($page, 50,  $y+10, 10);
    $form->CForm_Add_Text_Line   ($page, 53,  $y+10, FONT, 10, "Sportsman");
    $form->CForm_Add_Checkbox    ($page, 70,  $y+10, 10);
    $form->CForm_Add_Text_Line   ($page, 73,  $y+10, FONT, 10, "Intermediate");
    $form->CForm_Add_Checkbox    ($page, 91,  $y+10, 10);
    $form->CForm_Add_Text_Line   ($page, 94,  $y+10, FONT, 10, "Advanced");
    $form->CForm_Add_Checkbox    ($page, 109, $y+10, 10);
    $form->CForm_Add_Text_Line   ($page, 112, $y+10, FONT, 10, "Unlimited");
    $form->CForm_Add_Line ($page, 8, $y+14, 124);

    // Aircraft Information

    $y = 65;

    $form->CForm_Add_Text_Line   ($page, 8,   $y, FONT_BOLD, 10, "AIRCRAFT:");
    $form->CForm_Add_Text_Line   ($page, 22,  $y, FONT, 10, "Make/Model");
    $form->CForm_Add_UnderLine   ($page, 36,  $y, 81, 10);
    $form->CForm_Add_Text_Line   ($page, 86,  $y, FONT, 10, '"N" Number');
    $form->CForm_Add_UnderLine   ($page, 99,  $y, 123, 10);
    $form->CForm_Add_Line ($page, 8, $y+4, 124);

    // Aircraft Paperwork

    $form->CForm_Add_Text_Line   ($page, 8,  72, FONT_BOLD, 10, 
      "AIRCRAFT AND PILOT PAPERWORK");
    $form->CForm_Add_Checkbox    ($page, 8,  76, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 76, FONT, 10, 
      "IAC Membership Card or FAI Sporting License");
    $form->CForm_Add_Checkbox    ($page, 67, 76, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 76, FONT, 10, "Certificate of Airworthiness");
    $form->CForm_Add_Checkbox    ($page, 8,  81, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 81, FONT, 10, "Pilot Certificate");
    $form->CForm_Add_Checkbox    ($page, 67, 81, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 81, FONT, 10, 
      "Annual or Condition Inspection (Copy OK)");
    $form->CForm_Add_Checkbox    ($page, 8,  86, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 86, FONT, 10, "Medical (appropriate to aircraft type: LSA, glider, etc.)");
    $form->CForm_Add_Checkbox    ($page, 67, 86, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 86, FONT, 10, "Operating Limitations (Copy OK)");
    $form->CForm_Add_Checkbox    ($page, 8,  91, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 91, FONT, 10, "Registration");
    $form->CForm_Add_Checkbox    ($page, 67, 91, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 91, FONT, 10, "Parachute Log showing current pack (180 days)");
    $form->CForm_Add_Checkbox    ($page, 8,  96, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 96, FONT, 10, "Proof of Insurance (Copy OK)");
    $form->CForm_Add_Checkbox    ($page, 67, 96, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 96, FONT, 10, "Weight & Balance (Copy OK)");
    $form->CForm_Add_Line ($page, 8, 100, 124);

    // Mechnical Condition

    $y = 102;

    $form->CForm_Add_Text_Lines  ($page, 8, $y, 124, FONT_BOLD, 10,
    "MECHANICAL CONDITION (Inspect all items and mark only those that may ".
    "affect safety.  Once an item is corrected, note it on page 2)");

    // left column
    $form->CForm_Add_Checkbox    ($page, 8,  $y+7, 10);
    $form->CForm_Add_Text_Lines  ($page, 12, $y+7, 62, FONT, 10, 
      "Cockpit area (loose objects, seat belts, shoulder harness, ".
      "canopy security, windscreen, etc.)");

    $form->CForm_Add_Checkbox    ($page, 8,  $y+14, 10);
    $form->CForm_Add_Text_Lines  ($page, 12, $y+14, 62, FONT, 10, 
      "Ailerons (freedom of movement, excessive play, condition of hinges, etc.");

    $form->CForm_Add_Checkbox    ($page, 8,  $y+21, 10);
    $form->CForm_Add_Text_Lines  ($page, 12, $y+21, 62, FONT, 10, 
      "Flying and Landing wires / struts -- if applicable ".
      "(tension, condition, corrosion)");

    $form->CForm_Add_Checkbox    ($page, 8,  $y+28, 10);
    $form->CForm_Add_Text_Lines  ($page, 12, $y+28, 62, FONT, 10, 
      "Condition of wing fabric, metal, or composite surfaces ".
      "(wrinkles, popped rivets, loose tapes, etc.)");

    $form->CForm_Add_Checkbox    ($page, 8,  $y+37, 10);
    $form->CForm_Add_Text_Line   ($page, 12, $y+37, FONT, 10, 
      "Propeller and spinner (condition, security)");
    
    $form->CForm_Add_Checkbox    ($page, 8,  $y+42, 10);
    $form->CForm_Add_Text_Line   ($page, 12, $y+42, FONT, 10, 
      "Cowl (security)");

    $form->CForm_Add_Checkbox    ($page, 8,  $y+47, 10);
    $form->CForm_Add_Text_Lines  ($page, 12, $y+47, 62, FONT, 10, 
      "Unsafe loose or chafed wires or hoses (other hazards ".
      "observed through cowl openings)");

    $form->CForm_Add_Checkbox    ($page, 8,  $y+54, 10);
    $form->CForm_Add_Text_Lines  ($page, 12, $y+54, 62, FONT, 10, 
      "Fuel leaks (fuel must be leaking to disqualify - ".
      "stains should be brought to pilot's attention)");

    $form->CForm_Add_Checkbox    ($page, 8,  $y+61, 10);
    $form->CForm_Add_Text_Line   ($page, 12, $y+61, FONT, 10, 
      "Excessive oil leaks");

    // right column
    $form->CForm_Add_Checkbox    ($page, 67, $y+7, 10);
    $form->CForm_Add_Text_Line   ($page, 71, $y+7, FONT, 10, 
      "Exhaust (cracks, security)");

    $form->CForm_Add_Checkbox    ($page, 67, $y+12, 10);
    $form->CForm_Add_Text_Line   ($page, 71, $y+12, FONT, 10, 
      "Landing gear (condition)");

    $form->CForm_Add_Checkbox    ($page, 67, $y+17, 10);
    $form->CForm_Add_Text_Line   ($page, 71, $y+17, FONT, 10, 
      "Brakes (condition, fluid leaks)");

    $form->CForm_Add_Checkbox    ($page, 67, $y+22, 10);
    $form->CForm_Add_Text_Line   ($page, 71, $y+22, FONT, 10, 
      "Tires (inflation, excessive wear)");

    $form->CForm_Add_Checkbox    ($page, 67, $y+27, 10);
    $form->CForm_Add_Text_Line   ($page, 71, $y+27, FONT, 10, 
      "Wheel pants - if installed (security)");

    $form->CForm_Add_Checkbox    ($page, 67, $y+32, 10);
    $form->CForm_Add_Text_Lines  ($page, 71, $y+32, 124, FONT, 10, 
      "Condition of fuselage fabric, metal or composite surfaces ".
      "(wrinkles, popped rivets, loose tapes, etc.)");

    $form->CForm_Add_Checkbox    ($page, 67, $y+39, 10);
    $form->CForm_Add_Text_Line   ($page, 71, $y+39, FONT, 10, 
      "Loose or foreign objects in fuselage / tail");

    $form->CForm_Add_Checkbox    ($page, 67, $y+44, 10);
    $form->CForm_Add_Text_Line   ($page, 71, $y+44, FONT, 10, 
      "Tail attach fittings (no play, condition)");

    $form->CForm_Add_Checkbox    ($page, 67, $y+49, 10);
    $form->CForm_Add_Text_Line   ($page, 71, $y+49, FONT, 10, 
      "Tail wires or struts (if applicable)");
      
    $form->CForm_Add_Checkbox    ($page, 67, $y+54, 10);
    $form->CForm_Add_Text_Line   ($page, 71, $y+54, FONT, 10, 
      "Elevator and rudder (freedom of movement)");

    $form->CForm_Add_Checkbox    ($page, 67, $y+59, 10);
    $form->CForm_Add_Text_Line   ($page, 71, $y+59, FONT, 10, 
      "Elevator trim (condition, excessive play)");

    $form->CForm_Add_Checkbox    ($page, 67, $y+64, 10);
    $form->CForm_Add_Text_Line   ($page, 71, $y+64, FONT, 10, 
      "Tail wheel (condition, inflation, springs, security)");
}

function Make_Tech_Page2 (&$form, $page)
{
    $form->CForm_Add_Text_Lines  ($page, 8, 15, 124, FONT_BOLD, 10,
    "Items corrected from page 1: (Note each unsafe item and how corrected.  ".
    "All Unsafe items must be corrected prior to registration.)");

    $form->CForm_Add_Checkbox    ($page, 8,  26, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 26, FONT, 10, "None Noted");

    $form->CForm_Add_Checkbox    ($page, 8,  33, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 33, FONT, 10, "Unsafe items noted:");
    $form->CForm_Add_UnderLine   ($page, 8,  36, 123, 10);
    $form->CForm_Add_UnderLine   ($page, 8,  39, 123, 10);
    $form->CForm_Add_UnderLine   ($page, 8,  42, 123, 10);
    $form->CForm_Add_UnderLine   ($page, 8,  45, 123, 10);
    $form->CForm_Add_UnderLine   ($page, 8,  48, 123, 10);

    $form->CForm_Add_Text_Line   ($page, 8,  56, FONT, 10, "How Corrected:");
    $form->CForm_Add_UnderLine   ($page, 8,  59, 123, 10);
    $form->CForm_Add_UnderLine   ($page, 8,  62, 123, 10);
    $form->CForm_Add_UnderLine   ($page, 8,  65, 123, 10);
    $form->CForm_Add_UnderLine   ($page, 8,  68, 123, 10);
    $form->CForm_Add_UnderLine   ($page, 8,  71, 123, 10);

    // Signature Line

    $form->CForm_Add_Line        ($page, 8,  86, 64);
    $form->CForm_Add_Text_Line   ($page, 8,  87, FONT, 10, "Technical Inspector");
    $form->CForm_Add_Line        ($page, 70, 86, 100);
    $form->CForm_Add_Text_Line   ($page, 70, 87, FONT, 10, "Date");
}

function Split_Name ($name)
{
    $lines = array ();
	$len   = strlen ($name);
    if ($len > 30) {
	    $i = floor ($len/2);
		while ($i < $len && $name [$i] != ' ') {
		    $i++;
		}
		if ($i < $len) {
		    $lines [] = substr ($name, 0, $i);
			$lines [] = substr ($name, $i + 1);
		}
		else {
		    $lines [] = $name;
		}
	}
	else {
	    $lines [] = $name;
	}
	return $lines;
}

function Fill_Entry_Template (&$form, $fields, $page)
{
    $y = 30;

    $lines = Split_Name ($fields ["contest_name"]);
	if (count ($lines) > 1) {
        $form->CForm_Add_Text_Line ($page, 34, $y - 2, FILL_FONT, 10, $lines [0]);
        $form->CForm_Add_Text_Line ($page, 34, $y, FILL_FONT, 10, $lines [1]);
	}
	else {
        $form->CForm_Add_Text_Line ($page, 34, $y, FILL_FONT, 10, $fields ["contest_name"]);
	}
    $form->CForm_Add_Text_Line ($page, 100, $y, FILL_FONT, 10, Value ($fields, "contest_date"));
    $form->CForm_Add_Text_Line ($page, 34,  $y + 5, FILL_FONT, 10, Value ($fields, "contest_location"));
    switch (Value ($fields, "contest_type")) {
        case "Power":
            $form->CForm_Add_Xmark ($page, 100, $y + 5, 10);
            break;
        case "Glider":
            $form->CForm_Add_Xmark ($page, 115, $y + 5, 10);
            break;
    }

    $categories = explode (",", Value ($fields, "contest_category"));
    foreach ($categories as $category) {
		switch ($category) {
			case "Primary":
				$form->CForm_Add_Xmark ($page, 43, $y + 10, 10);
				break;
			case "Sportsman":
				$form->CForm_Add_Xmark ($page, 57, $y + 10, 10);
				break;
			case "Intermediate":
				$form->CForm_Add_Xmark ($page, 75, $y + 10, 10);
				break;
			case "Advanced":
				$form->CForm_Add_Xmark ($page, 95, $y + 10, 10);
				break;
			case "Unlimited":
				$form->CForm_Add_Xmark ($page, 112, $y + 10, 10);
				break;
		}
	}

    $categories = explode (",", Value ($fields, "contest_patch_category"));
    foreach ($categories as $category) {
		switch ($category) {
			case "Primary":
				$form->CForm_Add_Xmark ($page, 43, $y + 15, 10);
				break;
			case "Sportsman":
				$form->CForm_Add_Xmark ($page, 57, $y + 15, 10);
				break;
			case "Intermediate":
				$form->CForm_Add_Xmark ($page, 75, $y + 15, 10);
				break;
			case "Advanced":
				$form->CForm_Add_Xmark ($page, 95, $y + 15, 10);
				break;
			case "Unlimited":
				$form->CForm_Add_Xmark ($page, 112, $y + 15, 10);
				break;
		}
	}
	
	if (Value ($fields, "contest_first_time") == "first_time") {
	    $form->CForm_Add_Xmark ($page, 47, $y + 20, 10);
	}

    $y = $y + 28;

    $form->CForm_Add_Text_Line ($page, 32,  $y,     FILL_FONT, 10, Value ($fields, "pilot_first_name") . " " . Value ($fields, "pilot_last_name"));
    $form->CForm_Add_Text_Line ($page, 104, $y,     FILL_FONT, 10, Value ($fields, "pilot_iac_number"));
    $form->CForm_Add_Text_Line ($page, 32,  $y + 5, FILL_FONT, 10, Value ($fields, "pilot_street"));
    $form->CForm_Add_Text_Line ($page, 72,  $y + 5, FILL_FONT, 10, Value ($fields, "pilot_city"));
    $form->CForm_Add_Text_Line ($page, 104, $y + 5, FILL_FONT, 10, Value ($fields, "pilot_state") . " " . Value ($fields, "pilot_zip"));
    $form->CForm_Add_Text_Line ($page, 33, $y + 12, FILL_FONT, 10, Value ($fields, "pilot_country"));
    $y = $y + 5;
    $form->CForm_Add_Text_Line ($page, 40,  $y + 12, FILL_FONT, 10, Value ($fields, "pilot_email"));
    $form->CForm_Add_Text_Line ($page, 100, $y + 12, FILL_FONT, 10, Value ($fields, "pilot_birthdate"));
    $form->CForm_Add_Text_Line ($page, 32,  $y + 17, FILL_FONT, 10, Value ($fields, "pilot_phone"));
    $form->CForm_Add_Text_Line ($page, 88,  $y + 17, FILL_FONT, 10, Value ($fields, "pilot_cell"));
    $form->CForm_Add_Text_Line ($page, 40 , $y + 22, FILL_FONT, 10, Value ($fields, "pilot_chapter"));
    $form->CForm_Add_Text_Line ($page, 100, $y + 22, FILL_FONT, 10, Value ($fields, "pilot_fai_number"));
    $form->CForm_Add_Text_Line ($page, 40,  $y + 27, FILL_FONT, 10, Value ($fields, "pilot_license_type"));
    $form->CForm_Add_Text_Line ($page, 96,  $y + 27, FILL_FONT, 10, Value ($fields, "pilot_license_number"));
    $form->CForm_Add_Text_Line ($page, 45,  $y + 32, FILL_FONT, 10, Value ($fields, "pilot_emergency_contact"));
    $form->CForm_Add_Text_Line ($page, 86,  $y + 32, FILL_FONT, 10, Value ($fields, "pilot_emergency_contact_phone"));

    // Owner Information

    $y = $y + 42;

    $form->CForm_Add_Text_Line ($page, 32,  $y, FILL_FONT, 10, Value ($fields, "owner_name"));
    $form->CForm_Add_Text_Line ($page, 100, $y, FILL_FONT, 10, Value ($fields, "owner_phone"));
    $form->CForm_Add_Text_Line ($page, 32,  $y + 5, FILL_FONT, 10, Value ($fields, "owner_street"));
    $form->CForm_Add_Text_Line ($page, 72,  $y + 5, FILL_FONT, 10, Value ($fields, "owner_city"));
    $form->CForm_Add_Text_Line ($page, 104, $y + 5, FILL_FONT, 10, Value ($fields, "owner_state") . " " . Value ($fields, "owner_zip"));

    // Aircraft Information

    $y = $y + 15;

    $form->CForm_Add_Text_Line ($page, 38,  $y, FILL_FONT, 10, Value ($fields, "aircraft_make_model"));
    $form->CForm_Add_Text_Line ($page, 104, $y, FILL_FONT, 10, Value ($fields, "aircraft_n_number"));
    if (Value ($fields, "aircraft_grass_root_qualified") == "grass_root_qualified") {
        $form->CForm_Add_Xmark     ($page, 23,  $y + 5, 10);
    }

    switch (Value ($fields, "aircraft_airworthiness")) {
        case "Experimental":
            $form->CForm_Add_Xmark ($page, 92,  $y + 5, 10);
            break;
        case "Aerobatic":
            $form->CForm_Add_Xmark ($page, 113, $y + 5, 10);
            break;
    }

    $form->CForm_Add_Text_Line ($page, 58, $y + 10, FILL_FONT, 10,
                                Value ($fields, "aircraft_engine_make") . "/" .
                                Value ($fields, "aircraft_engine_model") . "/" .
                                Value ($fields, "aircraft_engine_horsepower"));

    $form->CForm_Add_Text_Line ($page, 46,  $y + 15, FILL_FONT, 10, Value ($fields, "aircraft_insurance_company"));
    $form->CForm_Add_Text_Line ($page, 80,  $y + 15, FILL_FONT, 10, Value ($fields, "aircraft_insurance_type"));
    $form->CForm_Add_Text_Line ($page, 103, $y + 15, FILL_FONT, 10, Value ($fields, "aircraft_insurance_amount"));
}

function Fill_Tech_Template (&$form, $fields, $page)
{
    $form->CForm_Add_Text_Line ($page, 34, 38, FILL_FONT, 10, Value ($fields, "contest_name"));
    $form->CForm_Add_Text_Line ($page, 34, 43, FILL_FONT, 10, Value ($fields, "contest_location"));
    $form->CForm_Add_Text_Line ($page, 96, 43, FILL_FONT, 10, Value ($fields, "contest_date"));

    $form->CForm_Add_Text_Line ($page, 32, 49, FILL_FONT, 10, Value ($fields, "pilot_first_name") . " " . Value ($fields, "pilot_last_name"));
    $form->CForm_Add_Text_Line ($page, 80, 49, FILL_FONT, 10, Value ($fields, "pilot_iac_number"));

    switch (Value ($fields, "contest_type")) {
        case "Power":
            $form->CForm_Add_Xmark ($page, 34, 54, 10);
            break;
        case "Glider":
            $form->CForm_Add_Xmark ($page, 50, 54, 10);
            break;
    }
    $categories = explode (",", Value ($fields, "contest_category"));
    foreach ($categories as $category) {
		switch ($category) {
			case "Primary":
				$form->CForm_Add_Xmark ($page, 34, 59, 10);
				break;
			case "Sportsman":
				$form->CForm_Add_Xmark ($page, 50, 59, 10);
				break;
			case "Intermediate":
				$form->CForm_Add_Xmark ($page, 70, 59, 10);
				break;
			case "Advanced":
				$form->CForm_Add_Xmark ($page, 91, 59, 10);
				break;
			case "Unlimited":
				$form->CForm_Add_Xmark ($page, 109, 59, 10);
				break;
		}
	}

    $form->CForm_Add_Text_Line ($page, 38, 65, FILL_FONT, 10, Value ($fields, "aircraft_make_model"));
    $form->CForm_Add_Text_Line ($page, 100, 65, FILL_FONT, 10, Value ($fields, "aircraft_n_number"));
}

function Make_Blank (&$form, $page)
{
    $form->CForm_Add_Text_Line ($page, 48, 84, FONT, 10, "This page is intentionally left blank");
}

function Make_Order_Of_Flight_Template (&$form, $page)
{
    // Logo

    $form->CForm_Add_Image ($page, 9, 6, 20, "logos/logo.png");

    // Header

    $form->CForm_Add_Text_Line   ($page, 45, 8, FONT_BOLD, 12, "ORDER OF FLIGHT WORKSHEET");

    // Category

    $form->CForm_Add_Checkbox  ($page,  9, 21, 10);
    $form->CForm_Add_Text_Line ($page, 11, 21, FONT_BOLD, 10, "Primary");
    $form->CForm_Add_Checkbox  ($page, 25, 21, 10);
    $form->CForm_Add_Text_Line ($page, 27, 21, FONT_BOLD, 10, "Sportsman");
    $form->CForm_Add_Checkbox  ($page, 47, 21, 10);
    $form->CForm_Add_Text_Line ($page, 49, 21, FONT_BOLD, 10, "Intermediate");
    $form->CForm_Add_Checkbox  ($page, 70, 21, 10);
    $form->CForm_Add_Text_Line ($page, 72, 21, FONT_BOLD, 10, "Advanced");
    $form->CForm_Add_Checkbox  ($page, 93, 21, 10);
    $form->CForm_Add_Text_Line ($page, 95, 21, FONT_BOLD, 10, "Unlimited");

    $form->CForm_Add_Checkbox  ($page,  9, 26, 10);
    $form->CForm_Add_Text_Line ($page, 11, 26, FONT_BOLD, 10, "Glider Sportsman");
    $form->CForm_Add_Checkbox  ($page, 38, 26, 10);
    $form->CForm_Add_Text_Line ($page, 40, 26, FONT_BOLD, 10, "Glider Intermediate");
    $form->CForm_Add_Checkbox  ($page, 70, 26, 10);
    $form->CForm_Add_Text_Line ($page, 72, 26, FONT_BOLD, 10, "Glider Advanced");
    $form->CForm_Add_Checkbox  ($page, 99, 26, 10);
    $form->CForm_Add_Text_Line ($page, 101, 26, FONT_BOLD, 10, "Glider Unlimited");

    // Pilot/Aircraft

    $form->CForm_Add_Text_Line ($page,  8, 32, FONT, 10, "Competitor Name:");
    $form->CForm_Add_Underline ($page, 29, 32, 128, 10);

    $form->CForm_Add_Text_Line ($page,  8, 38, FONT, 10, "Type Aircraft:");
    $form->CForm_Add_Underline ($page, 24, 38, 55, 10);
    $form->CForm_Add_Text_Line ($page, 65, 38, FONT, 10, "N-Number:");
    $form->CForm_Add_Underline ($page, 78, 38, 128, 10);

    $form->CForm_Add_Text_Line ($page,  8, 44, FONT, 10, 
      "Is anyone else flying this aircraft today?");
    $form->CForm_Add_Checkbox  ($page, 68, 44, 10);
    $form->CForm_Add_Text_Line ($page, 71, 44, FONT, 10, "Yes");
    $form->CForm_Add_Checkbox  ($page, 83, 44, 10);
    $form->CForm_Add_Text_Line ($page, 86, 44, FONT, 10, "No");

    $form->CForm_Add_Text_Line ($page,  8, 50, FONT, 10, "Who:");
    $form->CForm_Add_Underline ($page, 15, 50, 55, 10);
    $form->CForm_Add_Text_Line ($page, 60, 50, FONT, 10, "What Category:");
    $form->CForm_Add_Underline ($page, 78, 50, 128, 10);

    $form->CForm_Add_Text_Line ($page,  8, 56, FONT, 10, 
      "Are you a Safety Pilot for another competitor?");
    $form->CForm_Add_Checkbox  ($page, 68, 56, 10);
    $form->CForm_Add_Text_Line ($page, 71, 56, FONT, 10, "Yes");
    $form->CForm_Add_Checkbox  ($page, 83, 56, 10);
    $form->CForm_Add_Text_Line ($page, 86, 56, FONT, 10, "No");

    $form->CForm_Add_Text_Line ($page,  8, 62, FONT, 10, "Who:");
    $form->CForm_Add_Underline ($page, 15, 62, 55, 10);
    $form->CForm_Add_Text_Line ($page, 60, 62, FONT, 10, "What Category:");
    $form->CForm_Add_Underline ($page, 78, 62, 128, 10);

    // Registrar

    $form->CForm_Add_Text_Line ($page,  8, 68, FONT_BOLD, 10, "FOR REGISTRAR USE ONLY:");
    $form->CForm_Add_Text_Line ($page,  8, 74, FONT, 10, "KNOWN");
    $form->CForm_Add_Underline ($page, 19, 74, 29, 10);
    $form->CForm_Add_Text_Line ($page, 38, 74, FONT, 10, "UNKNOWN");
    $form->CForm_Add_Underline ($page, 53, 74, 63, 10);
    $form->CForm_Add_Text_Line ($page, 70, 74, FONT, 10, "FREE");
    $form->CForm_Add_Underline ($page, 78, 74, 88, 10);
    $form->CForm_Add_Text_Line ($page, 99, 74, FONT, 10, "4-MINUTE");
    $form->CForm_Add_Underline ($page, 115, 74, 125, 10);

    $form->CForm_Add_Line ($page, 8, 89, 128);
}

function Fill_Order_Of_Flight_Template (&$form, $fields, $page)
{
  $categories = explode (",", Value ($fields, "contest_category"));
  if (Value ($fields, "contest_type") == "Glider") {
		foreach ($categories as $category) {
			switch ($category) {
				case "Sportsman":
					$form->CForm_Add_Xmark ($page, 9, 26, 10);
					break;
				case "Intermediate":
					$form->CForm_Add_Xmark ($page, 38, 26, 10);
					break;
				case "Advanced":
					$form->CForm_Add_Xmark ($page, 70, 26, 10);
					break;
				case "Unlimited":
					$form->CForm_Add_Xmark ($page, 99, 26, 10);
					break;
			}
		}
  }
  else {
    foreach ($categories as $category) {
      switch ($category) {
        case "Primary":
          $form->CForm_Add_Xmark ($page,  9, 21, 10);
          break;
        case "Sportsman":
          $form->CForm_Add_Xmark ($page, 25, 21, 10);
          break;
        case "Intermediate":
          $form->CForm_Add_Xmark ($page, 47, 21, 10);
          break;
        case "Advanced":
          $form->CForm_Add_Xmark ($page, 70, 21, 10);
          break;
        case "Unlimited":
          $form->CForm_Add_Xmark ($page, 93, 21, 10);
          break;
      }
		}
	}

    $form->CForm_Add_Text_Line ($page, 30,  32, FILL_FONT, 10, 
       Value ($fields, "pilot_first_name") . " " . 
       Value ($fields, "pilot_last_name"));
    $form->CForm_Add_Text_Line ($page, 26,  38, FILL_FONT, 10,
       Value ($fields, "aircraft_make_model"));
    $form->CForm_Add_Text_Line ($page, 80,  38, FILL_FONT, 10,
       Value ($fields, "aircraft_n_number"));

    if (Value ($fields, "aircraft_other") == "Yes") {
        $form->CForm_Add_Xmark ($page, 68, 44, 10);
        $form->CForm_Add_Text_Line ($page, 17,  50, FILL_FONT, 10, 
           Value ($fields, "aircraft_other_who_else"));
        $form->CForm_Add_Text_Line ($page, 80,  50, FILL_FONT, 10, 
           Value ($fields, "aircraft_other_category"));
    }
    else {
        $form->CForm_Add_Xmark ($page, 83, 44, 10);
    }
}

function Make_Volunteer_Template (&$form, $page, $offset = 0)
{
    // Logo

    $form->CForm_Add_Image ($page, 9, 93 + $offset, 20, "logos/logo.png");

    // Header

    $form->CForm_Add_Text_Line ($page, 54, 95 + $offset, FONT_BOLD, 10, "VOLUNTEER SIGN-UP SHEET");
    $form->CForm_Add_Underline ($page, 54, 95 + $offset, 87, 10);

    // Name

    $form->CForm_Add_Text_Line ($page, 35,  104 + $offset, FONT, 10, "NAME:");
    $form->CForm_Add_Underline ($page, 46,  104 + $offset, 86, 10);
    $form->CForm_Add_Text_Line ($page, 60,  107 + $offset, FONT, 8, "(please print)");
    $form->CForm_Add_Text_Line ($page, 90,  104 + $offset, FONT, 10, "IAC #:");
    $form->CForm_Add_Underline ($page, 100, 104 + $offset, 120, 10);

    $form->CForm_Add_Text_Line ($page, 10, 112 + $offset, FONT, 10, "Are you flying in this contest?");
    $form->CForm_Add_Checkbox  ($page, 43, 112 + $offset, 10);
    $form->CForm_Add_Text_Line ($page, 46, 112 + $offset, FONT, 10, "Yes");
    $form->CForm_Add_Checkbox  ($page, 53, 112 + $offset, 10);
    $form->CForm_Add_Text_Line ($page, 56, 112 + $offset, FONT, 10, "No");
    $form->CForm_Add_Text_Line ($page, 64, 112 + $offset, FONT, 10, "Emergency Contact:");
    $form->CForm_Add_Underline ($page, 85, 112 + $offset, 120, 10);
    $form->CForm_Add_Text_Line ($page, 10, 118 + $offset, FONT, 10, "If Yes, what category?");
    $form->CForm_Add_Checkbox  ($page, 37, 118 + $offset, 10);
    $form->CForm_Add_Text_Line ($page, 40, 118 + $offset, FONT, 10, "Primary");
    $form->CForm_Add_Checkbox  ($page, 51, 118 + $offset, 10);
    $form->CForm_Add_Text_Line ($page, 54, 118 + $offset, FONT, 10, "Sportsman");
    $form->CForm_Add_Checkbox  ($page, 69, 118 + $offset, 10);
    $form->CForm_Add_Text_Line ($page, 72, 118 + $offset, FONT, 10, "Intermediate");
    $form->CForm_Add_Checkbox  ($page, 89, 118 + $offset, 10);
    $form->CForm_Add_Text_Line ($page, 92, 118 + $offset, FONT, 10, "Advanced");
    $form->CForm_Add_Checkbox  ($page, 106,118 + $offset, 10);
    $form->CForm_Add_Text_Line ($page, 109,118 + $offset, FONT, 10, "Unlimited");
    
    $form->CForm_Add_Text_Line ($page, 10, 124 + $offset, FONT, 10, "Are you a safety pilot for another competitor?");
    $form->CForm_Add_Checkbox  ($page, 58, 124 + $offset, 10);
    $form->CForm_Add_Text_Line ($page, 61, 124 + $offset, FONT, 10, "Yes");
    $form->CForm_Add_Checkbox  ($page, 68, 124 + $offset, 10);
    $form->CForm_Add_Text_Line ($page, 71, 124 + $offset, FONT, 10, "No");
    $form->CForm_Add_Text_Line ($page, 80, 124 + $offset, FONT, 10, "For whom?");
    $form->CForm_Add_Underline ($page, 92, 124 + $offset, 120, 10);
    
    $form->CForm_Add_Text_Line ($page, 10, 130 + $offset, FONT, 10,
        "Indicate where you would like to help. A contest is run with volunteers - we need every one of you!");

    // Position

    $form->CForm_Add_Text_Line ($page, 26, 134 + $offset, FONT, 10, "POSITION");
    $form->CForm_Add_Underline ($page, 26, 134 + $offset, 36, 10);
    $form->CForm_Add_Text_Line ($page, 76, 134 + $offset, FONT, 10, "CATEGORY PREFERENCE");
    $form->CForm_Add_Underline ($page, 76, 134 + $offset, 107, 10);
    $form->CForm_Add_Underline ($page, 11,  139 + $offset, 26, 10);
    $form->CForm_Add_Text_Line ($page, 26, 139 + $offset, FONT, 10, "Judge");
    $form->CForm_Add_Underline ($page, 76, 139 + $offset, 107, 10);
    $form->CForm_Add_Underline ($page, 11,  139 + $offset, 26, 10);
    $form->CForm_Add_Underline ($page, 11,  145 + $offset, 26, 10);
    $form->CForm_Add_Text_Line ($page, 26, 142 + $offset, FONT, 10, "Assistant Judge");
    $form->CForm_Add_Text_Line ($page, 26, 145 + $offset, FONT, 10, "(Must be able to read aerobatic figures)");
    $form->CForm_Add_Underline ($page, 76, 145 + $offset, 107, 10);
    $form->CForm_Add_Underline ($page, 11,  148 + $offset, 26, 10);
    $form->CForm_Add_Text_Line ($page, 26, 148 + $offset, FONT, 10, "Recorder");
    $form->CForm_Add_Underline ($page, 76, 148 + $offset, 107, 10);
    $form->CForm_Add_Underline ($page, 11,  151 + $offset, 26, 10);
    $form->CForm_Add_Text_Line ($page, 26, 151 + $offset, FONT, 10, "Boundary Judge");
    $form->CForm_Add_Underline ($page, 76, 151 + $offset, 107, 10);
    $form->CForm_Add_Underline ($page, 11,  154 + $offset, 26, 10);
    $form->CForm_Add_Text_Line ($page, 26, 154 + $offset, FONT, 10, "Score Runner");
    $form->CForm_Add_Underline ($page, 76, 154 + $offset, 107, 10);
    $form->CForm_Add_Underline ($page, 11,  157 + $offset, 26, 10);
    $form->CForm_Add_Text_Line ($page, 26, 157 + $offset, FONT, 10, "Timer/Radio");
    $form->CForm_Add_Underline ($page, 76, 157 + $offset, 107, 10);
    $form->CForm_Add_Underline ($page, 11,  160 + $offset, 26, 10);
    $form->CForm_Add_Text_Line ($page, 26, 160 + $offset, FONT, 10, "Assist Chief");
    $form->CForm_Add_Underline ($page, 76, 160 + $offset, 107, 10);
    $form->CForm_Add_Underline ($page, 11,  163 + $offset, 26, 10);
    $form->CForm_Add_Text_Line ($page, 26, 163 + $offset, FONT, 10, "Deadline");
    $form->CForm_Add_Underline ($page, 76, 163 + $offset, 107, 10);
}

function Get_Volunteer_Categories ($fields, $pos, $pos_val)
{
    $cat = "";
    if (Value ($fields, $pos . "_any") == $pos_val . "_any") {
        $cat = " ANY";
    }
    else {
        if (Value ($fields, $pos . "_p") == $pos_val . "_p") {
            $cat = " Primary";
        }
        if (Value ($fields, $pos . "_s") == $pos_val . "_s") {
            $cat = $cat . " Sportsman";
        }
        if (Value ($fields, $pos . "_i") == $pos_val . "_i") {
            $cat = $cat . " Intermediate";
        }
        if (Value ($fields, $pos . "_a") == $pos_val . "_a") {
            $cat = $cat . " Advanced";
        }
        if (Value ($fields, $pos . "_u") == $pos_val . "_u") {
            $cat = $cat . " Unlimited";
        }
    }
    return $cat;
}

function Fill_Volunteer_Template (&$form, $fields, $page, $offset = 0)
{
    $form->CForm_Add_Text_Line ($page, 47, 104 + $offset, FILL_FONT, 10, Value ($fields, "pilot_first_name") . " " . Value ($fields, "pilot_last_name"));
    $form->CForm_Add_Text_Line ($page, 101, 104 + $offset, FILL_FONT, 10, Value ($fields, "pilot_iac_number"));

    $categories = array_merge (
                      explode (",", Value ($fields, "contest_category")),
                      explode (",", Value ($fields, "contest_patch_category")));
    if ($fields['comp_type'] == 'competitor' && count ($categories) > 0)
    {
      $form->CForm_Add_Xmark ($page, 43, 112 + $offset, 10);
      foreach ($categories as $category) {
        switch ($category) {
          case "Primary":
            $form->CForm_Add_Xmark ($page, 37, 118 + $offset, 10);
            break;
          case "Sportsman":
            $form->CForm_Add_Xmark ($page, 51, 118 + $offset, 10);
            break;
          case "Intermediate":
            $form->CForm_Add_Xmark ($page, 69, 118 + $offset, 10);
            break;
          case "Advanced":
            $form->CForm_Add_Xmark ($page, 89, 118 + $offset, 10);
            break;
          case "Unlimited":
            $form->CForm_Add_Xmark ($page, 106, 118 + $offset, 10);
            break;
        }
      }
    }
    else {
        $form->CForm_Add_Xmark ($page, 53, 112 + $offset, 10);
    }
    
    $form->CForm_Add_Text_Line ($page, 86, 112 + $offset, FILL_FONT, 10, Value ($fields, "pilot_emergency_contact"));
    
    $safety_pilot_for = Value ($fields, "volunteer_safety_pilot_for");
    if ($safety_pilot_for != "") {
        $form->CForm_Add_Xmark ($page, 58, 124 + $offset, 10);
        $form->CForm_Add_Text_Line ($page, 93, 124 + $offset, FILL_FONT, 10, $safety_pilot_for);
    }
    else {
        $form->CForm_Add_Xmark ($page, 68, 124 + $offset, 10);
    }

    $cat = Value ($fields, "volunteer_judge");
    if ($cat != "") {
        $form->CForm_Add_Xmark ($page, 14, 139 + $offset, 10);
        $form->CForm_Add_Text_Line ($page, 76, 139 + $offset, FILL_FONT, 10, $cat);
    }
    $cat = Value ($fields, "volunteer_assist");
    if ($cat != "") {
        $form->CForm_Add_Xmark ($page, 14, 145 + $offset, 10);
        $form->CForm_Add_Text_Line ($page, 76, 145 + $offset, FILL_FONT, 10, $cat);
    }
    $cat = Value ($fields, "volunteer_recorder");
    if ($cat != "") {
        $form->CForm_Add_Xmark ($page, 14, 148 + $offset, 10);
        $form->CForm_Add_Text_Line ($page, 76, 148 + $offset, FILL_FONT, 10, $cat);
    }
    $cat = Value ($fields, "volunteer_boundary");
    if ($cat != "") {
        $form->CForm_Add_Xmark ($page, 14, 151 + $offset, 10);
        $form->CForm_Add_Text_Line ($page, 76, 151 + $offset, FILL_FONT, 10, $cat);
    }
    $cat = Value ($fields, "volunteer_runner");
    if ($cat != "") {
        $form->CForm_Add_Xmark ($page, 14, 154 + $offset, 10);
        $form->CForm_Add_Text_Line ($page, 76, 154 + $offset, FILL_FONT, 10, $cat);
    }
    $cat = Value ($fields, "volunteer_timer");
    if ($cat != "") {
        $form->CForm_Add_Xmark ($page, 14, 157 + $offset, 10);
        $form->CForm_Add_Text_Line ($page, 76, 157 + $offset, FILL_FONT, 10, $cat);
    }
    $cat = Value ($fields, "volunteer_assistChief");
    if ($cat != "") {
        $form->CForm_Add_Xmark ($page, 14, 160 + $offset, 10);
        $form->CForm_Add_Text_Line ($page, 76, 160 + $offset, FILL_FONT, 10, $cat);
    }
    $cat = Value ($fields, "volunteer_deadline");
    if ($cat != "") {
        $form->CForm_Add_Xmark ($page, 14, 163 + $offset, 10);
        $form->CForm_Add_Text_Line ($page, 76, 163 + $offset, FILL_FONT, 10, $cat);
    }
}

function Make_one_registration ($fields, $form, $index)
{
    $page = 6 * ($index - 1) + 1;
    Make_Entry_Template ($form, $page);
    Fill_Entry_Template ($form, $fields, $page);
    $page += 2;
    Make_Tech_Template   ($form, $page);
    Fill_Tech_Template   ($form, $fields, $page);
    Make_Tech_Page2 ($form, $page+1);
    $page += 2;
    Make_Order_Of_Flight_Template ($form, $page);
    Fill_Order_Of_Flight_Template ($form, $fields, $page);
    Make_Volunteer_Template ($form, $page);
    Fill_Volunteer_Template ($form, $fields, $page);
    $page += 1;
    Make_Blank ($form, $page);
}

function Make_one_volunteer($fields, $form, $index)
{
  $page = ($index - 1) / 2 + 1;
  $offset = ($index % 2) * -88;
  Make_Volunteer_Template($form, $page, $offset);
  Fill_Volunteer_Template($form, $fields, $page, $offset);
}

function Output_Form($form, $to_browser = TRUE)
{
    return $form->CForm_Make_Pdf (612, 792, 1.0, $to_browser);
}

function Make_Form ($fields, $to_browser = TRUE)
{
    $form = new CForm ();
    Make_one_registration ($form, $fields, 1);
    Output_Form($form, $to_browser);
}
?>
