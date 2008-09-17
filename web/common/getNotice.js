function getXmlHttpRequest(){
    var xml = null;
    try{
      xml = new XMLHttpRequest;
    }catch(e1){
      try{xml = new ActiveXObject("Msxml2.XMLHTTP");}
      catch(e2){xml = new ActiveXObject("Micriosft.XMLHTTP");}
    }
    return xml;
}
function getNotice(notice_id){
  var xml = getXmlHttpRequest();
  var url = "../common/getNotice.php";
  var handleStateChange = function(){
    if(xml.readyState == 4){
      var notice = document.getElementById(notice_id);
      if(xml.status == 200){
        if(xml.responseText.length > 0){
          notice.style.display = "block";
          notice.innerHTML = xml.responseText;
        }else{
          notice.style.display = "none";
        }
      }else{
        notice.style.display = "block";
        notice.innerHTML = "Unable to obtain notice, please try again later.";
      }
    }
  }
  try{
    xml.open("GET", url, true);
    xml.onreadystatechange = handleStateChange;
    xml.send(null);
  }catch(e){alert(e);}
}
