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
    $form->CForm_Add_Double_Line ($page, 21, 17, 128);

    // Contest Information
    
    $y = 20;

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
    $form->CForm_Add_Double_Line ($page, 8,   $y + 37, 128);

    // Owner Information
    
    $y = $y + 40;

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
    $form->CForm_Add_Double_Line ($page, 8,   $y + 10, 128);

    // Aircraft Information

    $y = $y + 13;
    
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
    $form->CForm_Add_Double_Line ($page, 8,   $y + 20, 128);

    // Legalese
    
    $y = $y + 23;

    $form->CForm_Add_Text_Lines ($page, 8, $y, 128, FONT, 10,
       "The undersigned owner and pilot hereby for the entry in the aforementioned contest and, if permitted to participate " .
       "in said contest, agree to the following terms and conditions:");

    $form->CForm_Add_Text_Lines ($page, 8,  $y + 8, 11,  FONT, 9, "1.");
    $form->CForm_Add_Text_Lines ($page, 12, $y + 8, 128, FONT, 9, "That all statements on this application are true;");

    $form->CForm_Add_Text_Lines ($page, 8,  $y + 13, 11,  FONT, 9, "2.");
    $form->CForm_Add_Text_Lines ($page, 12, $y + 13, 128, FONT, 9,
        "That the owner, pilot and aforementioned aircraft currently comply and will continue to comply throughout this contest " .
        "with all applicable Federal Aviation Regulations and all IAC rules and regulations;");

    $form->CForm_Add_Text_Lines ($page, 8,  $y + 20, 11,  FONT, 9, "3.");
    $form->CForm_Add_Text_Lines ($page, 12, $y + 20, 128, FONT, 9,
        "That participation in this contest is not authorized until acceptance is evidenced below by the contest director and the " .
        "entry fee has been paid;");

    $form->CForm_Add_Text_Lines ($page, 8,  $y + 27, 11,  FONT, 9, "4.");
    $form->CForm_Add_Text_Lines ($page, 12, $y + 27, 128, FONT, 9,
        "That all the requirements of the various contest committees will be immediately compiled with upon request, including " .
        "technical inspections and attendance at all pilot briefings;");

    $y = 8;
    
    $form->CForm_Add_Text_Lines ($page + 1, 8,  $y,  11, FONT, 9, "5.");
    $form->CForm_Add_Text_Lines ($page + 1, 12, $y, 128, FONT, 9,
        "That the undersigned assign and transfer any and all rights to photograph, tape, film, or in any way reproduce the " .
        "participation of owner, pilot and/or aircraft in this contest to the IAC, its agents, successors, or assigns;");

    $y = $y + 7;
    
    $form->CForm_Add_Text_Lines ($page + 1, 8,  $y, 11,  FONT, 9, "6.");
    $form->CForm_Add_Text_Lines ($page + 1, 12, $y, 128, FONT, 9,
        "That the undersigned, for themselves, their heirs, administrators, and executors, hereby accept full and complete " .
        "responsibility for and agree to release, indemnify, and hold the IAC, contest officials and sponsors, airport " .
        "owners and operators and any agents thereof harmless from any and all damages or liability whether caused by or " .
        "incurred by the undersigned as a result of their negligence, acts or omissions during participation in this contest;");

    $form->CForm_Add_Text_Lines ($page + 1, 8,  $y + 11, 11,  FONT, 9, "7.");
    $form->CForm_Add_Text_Lines ($page + 1, 12, $y + 11, 128, FONT, 9,
        "That the contest director may at any time and for any reason terminate participation of the owner, pilot, or aircraft " .
        "in this contest, and neither the director nor the IAC shall incur any liability or further duty to the undersigned as " .
        "a result thereof.");

    $form->CForm_Add_Text_Lines ($page + 1, 8,  $y + 19, 11, FONT, 9, "8.");
    $form->CForm_Add_Text_Lines ($page + 1, 12, $y + 19, 128, FONT, 9,
        "The International Aerobatic Club, Inc. as well as its chapters, officers, directors, employees, agents, contractors " .
        "and servants (all hereinafter referred to as �IAC�), are involved in varying degrees with the presentation of this " .
        "contest.  The IAC, HOWEVER, HAS ABSOLUTELY NO CONTROL NOR RESPONSIBILITY FOR ANY FLIGHT OR FLIGHT RELATED ACTIVITY " .
        "RESULTING FROM ANY INDIVIDUAL�S ATTENDANCE AT OR PARTICIPATION IN ANY PHASE OF SAID CONTEST.  ALL FLIGHT OR FLIGHT " .
        "RELATED ACTIVITY IS REGULATED BY THE RULES AND REGULATIONS OF THE FEDERAL AVIATION ADMINISTRATION and/or FIELD " .
        "PERSONNEL, INCLUDING BUT IN NO WAY LIMITED TO APPROACH, LANDING, TAXIING, PARKING, TAKEOFF, DEPARTURE, AND " .
        "PARTICIPATION IN SAID CONTEST.  ALL PILOTS OPERATE THEIR AIRCRAFT AS PILOT IN COMMAND, AT THEIR OWN RISK AND EACH " .
        "SUCH PILOT AS A CONDITION AND IN CONSIDERATION OF HIS OR HER ENGAGING IN SAID FLIGHT OR FLIGHT RELATED ACTIVITY THIS " .
        "CONTEST, FOR HIMSELF, HIS HEIRS AND ADMINISTRATORS, RELEASE AND AGREES TO INDEMNIFY AND HOLD THE IAC HARMLESS FROM " .
        "ANY AND ALL EXPENSES, DAMAGES, CAUSE OF ACTION, LOSSES, AND LIABILITY WHATSOEVER, WHETHER TO SAID PILOT OR TO " .
        "ANY PASSENGERS OR OTHER THIRD PARTIES.");

    $form->CForm_Add_Text_Lines ($page + 1, 8,  $y + 47, 11,  FONT, 9, "9.");
    $form->CForm_Add_Text_Lines ($page + 1, 12, $y + 47, 128, FONT, 9,
        "The permitting of participation by contestant ((owner and/or pilot) in no way shall be an approval or certification " .
        "of the structural or mechanical ability of any aircraft or the competency of any pilot to perform the maneuvers " .
        "necessary to participate in said contest.");

    // Pilot Signature

    $form->CForm_Add_Line      ($page + 1, 8,  $y + 57, 72);
    $form->CForm_Add_Text_Line ($page + 1, 8,  $y + 58, FONT, 10, "Pilot's Signature");
    $form->CForm_Add_Line      ($page + 1, 80, $y + 57, 126);
    $form->CForm_Add_Text_Line ($page + 1, 80, $y + 58, FONT, 10, "Date");

    // Owner Signature

    $form->CForm_Add_Line      ($page + 1, 8,  $y + 64, 72);
    $form->CForm_Add_Text_Line ($page + 1, 8,  $y + 65, FONT, 10, "Owner's Signature");
    $form->CForm_Add_Line      ($page + 1, 80, $y + 64, 126);
    $form->CForm_Add_Text_Line ($page + 1, 80, $y + 65, FONT, 10, "Date");

    $form->CForm_Add_Text_Line ($page + 1, 8, $y + 71, FONT_BOLD, 10, "ACCEPTED FOR PARTICIPATION:");

    // CD Signature

    $form->CForm_Add_Line      ($page + 1, 8, $y + 78, 72);
    $form->CForm_Add_Text_Line ($page + 1, 8, $y + 79, FONT, 10, "Contest Director's Signature");
    $form->CForm_Add_Line      ($page + 1, 80, $y + 78, 126);
    $form->CForm_Add_Text_Line ($page + 1, 80, $y + 79, FONT, 10, "Date");
    
    // Document info
    
    $form->CForm_Add_Text_Line ($page, 104, 170, FONT, 6, "IAC Official Contest Entry Form - 2010");
    $form->CForm_Add_Text_Line ($page + 1, 104, 170, FONT, 6, "IAC Official Contest Entry Form - 2010");
}

function Make_Tech_Template (&$form, $page)
{
    // Logo

    $form->CForm_Add_Image ($page, 10, 6, 20, "logos/logo.png");

    // Header

    $form->CForm_Add_Text_Line   ($page, 43, 8, FONT_BOLD, 12, "INTERNATIONAL AEROBATIC CLUB");
    $form->CForm_Add_Text_Line   ($page, 45, 12, FONT, 12, "Official Contest Technical Inspection");
    $form->CForm_Add_Double_Line ($page, 8, 19, 124);

    $form->CForm_Add_Text_Lines  ($page, 8, 21, 124, FONT, 9,
       "The following checklist is to be used for conducting technical " .
       "inspections and meets the entrance requirements for contest or " .
       "rally participation. NOTE: The Technical Inspection comprises a " .
       "thorough 'walk around' and is not a condition inspection. Proof " .
       "of a current Annual or Condition Inspection is considered " .
       "adequate evidence of compliance with all applicable regulatory " .
       "inspections, AD's and Service Bulletins. Removal of cowlings, " .
       "inspection plates, etc., will not be required unless there is " .
       "obvious evidence of an underlying problem. Excessive or unfair " .
       "inspections may be the subject of a Protest.");
    $form->CForm_Add_Double_Line ($page, 8, 34, 124);

    // Contest Information

    $form->CForm_Add_Text_Line   ($page, 8,   37, FONT_BOLD, 10, "CONTEST:");
    $form->CForm_Add_Text_Line   ($page, 22,  37, FONT, 10, "Name:");
    $form->CForm_Add_UnderLine   ($page, 34,  37, 80, 10);
    $form->CForm_Add_Text_Line   ($page, 86,  37, FONT, 10, "Date:");
    $form->CForm_Add_UnderLine   ($page, 94,  37, 123, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  42, FONT, 10, "Location:");
    $form->CForm_Add_UnderLine   ($page, 34,  42, 80, 10);
    $form->CForm_Add_Double_Line ($page, 8,   46, 124);

    // Pilot Information

    $form->CForm_Add_Text_Line   ($page, 8,   48, FONT_BOLD, 10, "PILOT:");
    $form->CForm_Add_Text_Line   ($page, 22,  48, FONT, 10, "Name:");
    $form->CForm_Add_UnderLine   ($page, 30,  48, 68, 10);
    $form->CForm_Add_Text_Line   ($page, 72,  48, FONT, 10, "IAC #:");
    $form->CForm_Add_UnderLine   ($page, 80,  48, 96, 10);
    $form->CForm_Add_Text_Line   ($page, 98,  48, FONT, 10, "Please Initial:");
    $form->CForm_Add_UnderLine   ($page, 112, 48, 123, 10);
    $form->CForm_Add_Text_Line   ($page, 22,  53, FONT, 10, "Type:");
    $form->CForm_Add_Checkbox    ($page, 34,  53, 10);
    $form->CForm_Add_Text_Line   ($page, 37,  53, FONT, 10, "Power");
    $form->CForm_Add_Checkbox    ($page, 50,  53, 10);
    $form->CForm_Add_Text_Line   ($page, 53,  53, FONT, 10, "Glider");
    $form->CForm_Add_Text_Line   ($page, 22,  58, FONT, 10, "Category:");
    $form->CForm_Add_Checkbox    ($page, 34,  58, 10);
    $form->CForm_Add_Text_Line   ($page, 37,  58, FONT, 10, "Primary");
    $form->CForm_Add_Checkbox    ($page, 50,  58, 10);
    $form->CForm_Add_Text_Line   ($page, 53,  58, FONT, 10, "Sportsman");
    $form->CForm_Add_Checkbox    ($page, 70,  58, 10);
    $form->CForm_Add_Text_Line   ($page, 73,  58, FONT, 10, "Intermediate");
    $form->CForm_Add_Checkbox    ($page, 91,  58, 10);
    $form->CForm_Add_Text_Line   ($page, 94,  58, FONT, 10, "Advanced");
    $form->CForm_Add_Checkbox    ($page, 109, 58, 10);
    $form->CForm_Add_Text_Line   ($page, 112, 58, FONT, 10, "Unlimited");
    $form->CForm_Add_Double_Line ($page, 8,   62, 124);

    // Aircraft Information

    $form->CForm_Add_Text_Line   ($page, 8,   64, FONT_BOLD, 10, "AIRCRAFT:");
    $form->CForm_Add_Text_Line   ($page, 22,  64, FONT, 10, "Make/Model");
    $form->CForm_Add_UnderLine   ($page, 36,  64, 81, 10);
    $form->CForm_Add_Text_Line   ($page, 86,  64, FONT, 10, '"N" Number');
    $form->CForm_Add_UnderLine   ($page, 99,  64, 123, 10);
    $form->CForm_Add_Double_Line ($page, 8,   68, 124);

    // Aircraft Paperwork

    $form->CForm_Add_Text_Line   ($page, 8,  71, FONT_BOLD, 10, "AIRCRAFT PAPERWORK");
    $form->CForm_Add_Checkbox    ($page, 8,  75, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 75, FONT, 10, "IAC Membership Card");
    $form->CForm_Add_Checkbox    ($page, 67, 75, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 75, FONT, 10, "Certificate of Airworthiness");
    $form->CForm_Add_Checkbox    ($page, 8,  79, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 79, FONT, 10, "Recreational Pilot Certificate");
    $form->CForm_Add_Checkbox    ($page, 67, 79, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 79, FONT, 10, "Annual or Condition Inspection (Copy OK)");
    $form->CForm_Add_Checkbox    ($page, 8,  83, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 83, FONT, 10, "Medical");
    $form->CForm_Add_Checkbox    ($page, 67, 83, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 83, FONT, 10, "Operating Limitations (Copy OK)");
    $form->CForm_Add_Checkbox    ($page, 8,  87, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 87, FONT, 10, "Registration");
    $form->CForm_Add_Checkbox    ($page, 67, 87, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 87, FONT, 10, "Parachute Log - Current Pack (Original - per FAA)");
    $form->CForm_Add_Checkbox    ($page, 8,  91, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 91, FONT, 10, "Proof of Insurance (Copy OK)");
    $form->CForm_Add_Checkbox    ($page, 67, 91, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 91, FONT, 10, "Weight & Balance (Computed for Competition)");
    $form->CForm_Add_Double_Line ($page, 8,  94, 124);

    // Mechnical Condition

    $form->CForm_Add_Text_Line   ($page, 8,  96, FONT_BOLD, 10, "MECHANICAL CONDITION (Check items that are hazardous or may affect safety)");
    $form->CForm_Add_Checkbox    ($page, 8,  101, 10);
    $form->CForm_Add_Text_Lines  ($page, 12, 101, 62, FONT, 10, "Cockpit area (Loose objects, seat belts, shoulder harness, canopy security, windscreen, etc.)");
    $form->CForm_Add_Checkbox    ($page, 67, 101, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 101, FONT, 10, "Cowl (security, evidence of leaks)");
    $form->CForm_Add_Checkbox    ($page, 8,  107, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 107, FONT, 10, "Play in any tail attach fittings (shake test)");
    $form->CForm_Add_Checkbox    ($page, 67, 105, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 105, FONT, 10, "Exhaust (cracks, security)");
    $form->CForm_Add_Checkbox    ($page, 8,  111, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 111, FONT, 10, "Elevator and rudder (freedom of movement)");
    $form->CForm_Add_Checkbox    ($page, 67, 109, 10);
    $form->CForm_Add_Text_Lines  ($page, 71, 109, 124, FONT, 10, "Fuel leaks (fuel must be leaking to disqualify; stains should be brought to pilot's attention)");
    $form->CForm_Add_Checkbox    ($page, 8,  115, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 115, FONT, 10, "Elevator trim condition (excessive play)");
    $form->CForm_Add_Checkbox    ($page, 67, 115, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 115, FONT, 10, "Excessive oil leaks");
    $form->CForm_Add_Checkbox    ($page, 8,  119, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 119, FONT, 10, "Tail wires or struts (if installed)");
    $form->CForm_Add_Checkbox    ($page, 67, 119, 10);
    $form->CForm_Add_Text_Lines  ($page, 71, 119, 124, FONT, 10, "Unsafe loose of chafed wires or hoses (other hazards observed through cowl openings)");
    $form->CForm_Add_Checkbox    ($page, 8,  123, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 123, FONT, 10, "Loose or foreign objects in tail");
    $form->CForm_Add_Checkbox    ($page, 67, 125, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 125, FONT, 10, "Landing gear (condition)");
    $form->CForm_Add_Checkbox    ($page, 8,  127, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 127, FONT, 10, "Leading edge (condition)");
    $form->CForm_Add_Checkbox    ($page, 67, 129, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 129, FONT, 10, "Tail wheel - Inflation, steering springs");
    $form->CForm_Add_Checkbox    ($page, 8,  131, 10);
    $form->CForm_Add_Text_Lines  ($page, 12, 131, 62, FONT, 10, "Wringles in fuselage fabric, metal or composite surface (structural problems?)");
    $form->CForm_Add_Checkbox    ($page, 67, 133, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 133, FONT, 10, "Brakes (condition, leaks)");
    $form->CForm_Add_Checkbox    ($page, 8,  137, 10);
    $form->CForm_Add_Text_Lines  ($page, 12, 137, 62, FONT, 10, "Condition of wing fabric, metal, or composite covering (wringles, popped rivets, loose tapes, etc.)");
    $form->CForm_Add_Checkbox    ($page, 67, 137, 10);
    $form->CForm_Add_Text_Lines  ($page, 71, 137, 124, FONT, 10, "Tires inflation & condition (Zlin 50, Sukoi, Yak-55, etc tires will show high wear. If no chord shows - acceptable.)");
    $form->CForm_Add_Checkbox    ($page, 8,  144, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 144, FONT, 10, "Ailerons (freedom of movement, excessive play");
    $form->CForm_Add_Checkbox    ($page, 67, 145, 10);
    $form->CForm_Add_Text_Line   ($page, 71, 145, FONT, 10, "Wheel pants - if installed (security)");
    $form->CForm_Add_Checkbox    ($page, 8,  148, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 148, FONT, 10, "Flying wires/struts, (tension, condition, corrosion)");
    $form->CForm_Add_Checkbox    ($page, 8,  152, 10);
    $form->CForm_Add_Text_Line   ($page, 12, 152, FONT, 10, "Propeller and spinner (condition, security)");

    // Signature Line

    $form->CForm_Add_Line        ($page, 8,  160, 64);
    $form->CForm_Add_Line        ($page, 70, 160, 120);
    $form->CForm_Add_Text_Line   ($page, 8,  161, FONT, 10, "Technical Inspector");
    $form->CForm_Add_Text_Line   ($page, 70, 161, FONT, 10, "Date");

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
    $y = 20;
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

    $y = $y + 40;
    $form->CForm_Add_Text_Line ($page, 32,  $y, FILL_FONT, 10, Value ($fields, "owner_name"));
    $form->CForm_Add_Text_Line ($page, 100, $y, FILL_FONT, 10, Value ($fields, "owner_phone"));
    $form->CForm_Add_Text_Line ($page, 32,  $y + 5, FILL_FONT, 10, Value ($fields, "owner_street"));
    $form->CForm_Add_Text_Line ($page, 72,  $y + 5, FILL_FONT, 10, Value ($fields, "owner_city"));
    $form->CForm_Add_Text_Line ($page, 104, $y + 5, FILL_FONT, 10, Value ($fields, "owner_state") . " " . Value ($fields, "owner_zip"));

    $y = $y + 13;
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
    $lines = Split_Name (Value ($fields, "contest_name"));
	if (count ($lines) > 1) {
        $form->CForm_Add_Text_Line ($page, 34, 35, FILL_FONT, 10, $lines [0]);
        $form->CForm_Add_Text_Line ($page, 34, 37, FILL_FONT, 10, $lines [1]);
	}
	else {
        $form->CForm_Add_Text_Line ($page, 34, 37, FILL_FONT, 10, Value ($fields, "contest_name"));
	}
    $form->CForm_Add_Text_Line ($page, 96, 37, FILL_FONT, 10, Value ($fields, "contest_date"));
    $form->CForm_Add_Text_Line ($page, 34, 42, FILL_FONT, 10, Value ($fields, "contest_location"));

    $form->CForm_Add_Text_Line ($page, 32, 48, FILL_FONT, 10, Value ($fields, "pilot_first_name") . " " . Value ($fields, "pilot_last_name"));
    $form->CForm_Add_Text_Line ($page, 80, 48, FILL_FONT, 10, Value ($fields, "pilot_iac_number"));

    switch (Value ($fields, "contest_type")) {
        case "Power":
            $form->CForm_Add_Xmark ($page, 34, 53, 10);
            break;
        case "Glider":
            $form->CForm_Add_Xmark ($page, 50, 53, 10);
            break;
    }
    $categories = explode (",", Value ($fields, "contest_category"));
    foreach ($categories as $category) {
		switch ($category) {
			case "Primary":
				$form->CForm_Add_Xmark ($page, 34, 58, 10);
				break;
			case "Sportsman":
				$form->CForm_Add_Xmark ($page, 50, 58, 10);
				break;
			case "Intermediate":
				$form->CForm_Add_Xmark ($page, 70, 58, 10);
				break;
			case "Advanced":
				$form->CForm_Add_Xmark ($page, 91, 58, 10);
				break;
			case "Unlimited":
				$form->CForm_Add_Xmark ($page, 109, 58, 10);
				break;
		}
	}

    $form->CForm_Add_Text_Line ($page, 38, 64, FILL_FONT, 10, Value ($fields, "aircraft_make_model"));
    $form->CForm_Add_Text_Line ($page, 100, 64, FILL_FONT, 10, Value ($fields, "aircraft_n_number"));

}

function Make_Blank (&$form, $page)
{
    $form->CForm_Add_Text_Line ($page, 48, 84, FONT, 10, "This page is intentionally left blank");
}

function Make_Order_Of_Flight_Template (&$form, $page)
{
    // Logo

    $form->CForm_Add_Image ($page, 9, 13, 20, "logos/logo.png");

    // Header

    $form->CForm_Add_Text_Line ($page, 28, 10, FONT, 10, "ORDER OF FLIGHT WORKSHEET");

    // Category

    $form->CForm_Add_Text_Line ($page, 28, 15, FONT, 10, "Primary");
    $form->CForm_Add_Underline ($page, 38, 15, 56, 10);
    $form->CForm_Add_Text_Line ($page, 70, 15, FONT, 10, "Advanced");
    $form->CForm_Add_Underline ($page, 82, 15, 99, 10);
    $form->CForm_Add_Text_Line ($page, 28, 20, FONT, 10, "Sportsman");
    $form->CForm_Add_Underline ($page, 41, 20, 56, 10);
    $form->CForm_Add_Text_Line ($page, 70, 20, FONT, 10, "Unlimited");
    $form->CForm_Add_Underline ($page, 82, 20, 99, 10);
    $form->CForm_Add_Text_Line ($page, 28, 25, FONT, 10, "Intermediate");
    $form->CForm_Add_Underline ($page, 43, 25, 56, 10);
    $form->CForm_Add_Text_Line ($page, 70, 25, FONT, 10, "Glider");
    $form->CForm_Add_Text_Line ($page, 81, 25, FONT, 10, "S");
    $form->CForm_Add_Text_Line ($page, 89, 25, FONT, 10, "I");
    $form->CForm_Add_Text_Line ($page, 97, 25, FONT, 10, "U");
    $form->CForm_Add_Text_Line ($page, 104, 25, FONT, 10, "(circle one)");

    // Pilot/Aircraft

    $form->CForm_Add_Text_Line ($page,  8, 32, FONT, 10, "Competitor Name");
    $form->CForm_Add_Underline ($page, 29, 32, 128, 10);
    $form->CForm_Add_Text_Line ($page,  8, 38, FONT, 10, "Type Aircraft/N-Number");
    $form->CForm_Add_Underline ($page, 35, 38, 128, 10);
    $form->CForm_Add_Text_Line ($page,  8, 44, FONT, 10, "Is Anyone Else Flying This Aircraft?");
    $form->CForm_Add_Underline ($page, 49, 44, 56, 10);
    $form->CForm_Add_Text_Line ($page, 58, 44, FONT, 10, "Yes");
    $form->CForm_Add_Underline ($page, 65, 44, 73, 10);
    $form->CForm_Add_Text_Line ($page, 75, 44, FONT, 10, "No");
    $form->CForm_Add_Text_Line ($page,  8, 50, FONT, 10, "Who");
    $form->CForm_Add_Underline ($page, 14, 50, 56, 10);
    $form->CForm_Add_Text_Line ($page, 62, 50, FONT, 10, "What Category");
    $form->CForm_Add_Underline ($page, 80, 50, 128, 10);

    // Registrar

    $form->CForm_Add_Text_Line ($page,  8, 56, FONT, 10, "FOR REGISTRAR USE ONLY:");
    $form->CForm_Add_Text_Line ($page,  8, 62, FONT, 10, "KNOWN");
    $form->CForm_Add_Underline ($page, 20, 62, 30, 10);
    $form->CForm_Add_Text_Line ($page, 55, 62, FONT, 10, "UNKNOWN");
    $form->CForm_Add_Underline ($page, 74, 62, 83, 10);
    $form->CForm_Add_Text_Line ($page,  8, 68, FONT, 10, "FREE");
    $form->CForm_Add_Underline ($page, 20, 68, 30, 10);
    $form->CForm_Add_Text_Line ($page, 55, 68, FONT, 10, "4-MIN");
    $form->CForm_Add_Underline ($page, 74, 68, 83, 10);

    // Note

    $form->CForm_Add_Text_Lines  ($page, 8, 74, 128, FONT_ITALIC, 11,
       "Note: Gliders sometimes have more than one Known, Free, or Unknown. Simply use a slash mark on " .
       "the line above to differentiate the order of flight for each");

    $form->CForm_Add_Line ($page, 8, 89, 128);

}

function Fill_Order_Of_Flight_Template (&$form, $fields, $page)
{
    $categories = explode (",", Value ($fields, "contest_category"));
    foreach ($categories as $category) {
		switch ($category) {
			case "Primary":
				$form->CForm_Add_Xmark ($page, 50, 15, 10);
				break;
			case "Sportsman":
				$form->CForm_Add_Xmark ($page, 50, 20, 10);
				break;
			case "Intermediate":
				$form->CForm_Add_Xmark ($page, 50, 25, 10);
				break;
			case "Advanced":
				$form->CForm_Add_Xmark ($page, 88, 15, 10);
				break;
			case "Unlimited":
				$form->CForm_Add_Xmark ($page, 88, 20, 10);
				break;
		}
	}
    if (Value ($fields, "contest_type") == "Glider") {
		$categories = explode (",", Value ($fields, "contest_category"));
		foreach ($categories as $category) {
			switch ($category) {
				case "Sportsman":
					$form->CForm_Add_Circle ($page, 81, 25, 10);
					break;
				case "Intermediate":
					$form->CForm_Add_Circle ($page, 89, 25, 10);
					break;
				case "Unlimited":
					$form->CForm_Add_Circle ($page, 97, 25, 10);
					break;
			}
		}
    }

    $form->CForm_Add_Text_Line ($page, 30,  32, FILL_FONT, 10, Value ($fields, "pilot_first_name") . " " . Value ($fields, "pilot_last_name"));
    $form->CForm_Add_Text_Line ($page, 36,  38, FILL_FONT, 10,
                               Value ($fields, "aircraft_make_model") . "/" .
                               Value ($fields, "aircraft_n_number"));

    if (Value ($fields, "aircraft_other") == "Yes") {
        $form->CForm_Add_Xmark ($page, 52, 44, 10);
        $form->CForm_Add_Text_Line ($page, 15,  50, FILL_FONT, 10, Value ($fields, "aircraft_other_who_else"));
        $form->CForm_Add_Text_Line ($page, 81,  50, FILL_FONT, 10, Value ($fields, "aircraft_other_category"));
    }
    else {
        $form->CForm_Add_Xmark ($page, 68, 44, 10);
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
    if ($fields['compType'] == 'competitor' && count ($categories) > 0)
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
    Make_Tech_Template   ($form, $page+2);
    Fill_Tech_Template   ($form, $fields, $page+2);
    Make_Blank ($form, $page+3);
    Make_Order_Of_Flight_Template ($form, $page+4);
    Fill_Order_Of_Flight_Template ($form, $fields, $page+4);
    Make_Volunteer_Template ($form, $page+4);
    Fill_Volunteer_Template ($form, $fields, $page+4);
    Make_Blank ($form, $page+5);
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
