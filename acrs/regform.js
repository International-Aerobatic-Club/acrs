function copyRegToOwner()
{
   var form = document.forms[0];
   if (form.ownerPilot.checked)
   { 
      form.ownerName.value = form.givenName.value + " " + form.familyName.value;
      form.ownerAddress.value = form.address.value;
      form.ownerCity.value = form.city.value;
      form.ownerState.value = form.state.value;
      form.ownerCountry.value = form.country.value;
      form.ownerPostal.value = form.postalCode.value;
      form.ownerPhone.value = form.contactPhone.value;
   }
}

function setIsDisabledOwner(form, disable)
{
   form.ownerName.disabled = disable;
   form.ownerAddress.disabled = disable;
   form.ownerCity.disabled = disable;
   form.ownerState.disabled = disable;
   form.ownerCountry.disabled = disable;
   form.ownerPostal.disabled = disable;
   form.ownerPhone.disabled = disable;
}

function enableAllForPost()
{
   setIsDisabledOwner(document.forms[0], false);
   setEnabled(document.getElementById("competitor"), true, false);
   setEnabled(document.getElementById("team"), true, false);
   setEnabled(document.getElementById("student"), true, false);
   setEnabled(document.getElementById("fourMinute"), true, false);
}

function checkOwnerPilot()
{
   var form = document.forms[0];
   var ownerPilot = form.ownerPilot;
   if (ownerPilot.checked)
   {
      copyRegToOwner();
   }
   setIsDisabledOwner(form, ownerPilot.checked);
}

/**
* Disable any and all input descendants of the parent
*/
function setEnabled(parent, isEnabled, doUncheck)
{
   for (var i = 0; i < parent.childNodes.length; ++i)
   {
      var child = parent.childNodes[i];
      if (child.tagName == "INPUT")
      {
        if (!isEnabled && doUncheck)
        {
           child.checked = false;
        }
        child.disabled = !isEnabled;
      }
      else
      {
        setEnabled(child, isEnabled, doUncheck);
      }
   }
}

function setEnabledCompetitor(hasTeam, allowsStudent, allows4Min)
{
   var isCompetitor = document.forms[0].compType[1].checked;
   setEnabled(document.getElementById("competitor"), isCompetitor, false);
   if (isCompetitor)
   {
      setEnabledForCategory(hasTeam, allowsStudent, allows4Min);
   }
   else
   {
      setEnabledForCategory(false, false, false);
   }
}

function setEnabledForCategory(hasTeam, allowsStudent, allows4Min)
{
   setEnabled(document.getElementById("team"), hasTeam, true);
   setEnabled(document.getElementById("student"), allowsStudent, true);
   setEnabled(document.getElementById("fourMinute"), allows4Min, true);
}
