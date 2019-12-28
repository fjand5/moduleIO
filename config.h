#define QUOTE(...) #__VA_ARGS__
char * config_page_befor = QUOTE(

<meta charset='utf-8'>
<html>
<head>
<title>Config system</title>
</head>
<body>
Config File:  
<br>
<textarea id='content'  style='height:90%; width:90%'>
);

char * config_page_after = QUOTE(

</textarea>
<br>
<button onclick='sendData()'>Áp dụng</button>
</form>
</body>

<script>
function sendData()
{
		
		var blob = new Blob([document.getElementById('content').value], {type: 'text/plain'});
	    var formData = new FormData();
	    var xhttp = new XMLHttpRequest();
		xhttp.open('POST','/config');
	    formData.append('',blob);
	    xhttp.send(formData);
	    

}
</script>

</html>

);
