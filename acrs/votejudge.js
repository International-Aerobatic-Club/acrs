// Functions support judge ballot

var regionsCount = new Array();
var totalCount = 0;

// TODO based on configuration
var maxTotal = 7;
var maxPerRegion = 2;

// Clear all checks
function clearVotes(form)
{
   for (var i = 0; i < form.elements.length; ++i)
   {
     var input = form.elements[i];
     input.checked = false;
   }
}

function initVoting()
{
  var form = document.forms[0];
  clearVotes(form);
  regionsCount['northeast'] = 0;
  regionsCount['southeast'] = 0;
  regionsCount['northwest'] = 0;
  regionsCount['southwest'] = 0;
  regionsCount['midamerica'] = 0;
  regionsCount['southcentral'] = 0;
  totalCount = 0;
}

function setDisplay(div, isShown)
{
   if(isShown)
   {  
      div.style.display="block";
   }
   else
   {
      div.style.display="none";
   }
}

function checkEnabledSubmit()
{
  var form = document.forms[0];
  var ctsOK = true;
  // TODO optional check based on configuration
//  for (var i in regionsCount)
//  {
//    ctsOK &= regionsCount[i] <= maxPerRegion;
//  }
  var ttlOK = totalCount <= maxTotal;
  form.submit.disabled = !(ctsOK && ttlOK);
  setDisplay(document.getElementById("ttlWarning"), !ttlOK);
  setDisplay(document.getElementById("rgnCtWarning"), !ctsOK);
}

// update count of judges, region judges
function checkJudge(input, region)
{
   if (input.checked)
   {
      totalCount += 1;
      regionsCount[region] += 1;
   }
   else
   {
      totalCount -= 1;
      regionsCount[region] -= 1;
   }
   checkEnabledSubmit();
}
