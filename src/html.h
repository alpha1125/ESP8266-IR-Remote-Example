//
// Created by Lloyd Leung on 2020-05-18.
//


String rowDiv = "    <div class=\"row\">\n";
String endDiv = "    </div>\n";




char buttonTemplate[] = R"=====(
    <div class="%s" style="text-align: center">
      <button id="%s" type="button" class="%s" onclick='makeAjaxCall("%s")'>%s</button>
    </div>
)=====";

char htmlHeader[] = R"=====(<!DOCTYPE html>
<html>
  <head>
    <title>Sharp TV Remote</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <meta name="HandheldFriendly" content="true">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css">
<style>
html, body {
//    touch-action:none;
    -webkit-text-size-adjust: none;
}
</style>
  </head>
  <body>
    <div class="container-fluid">
)=====";

char htmlFooter[] = R"=====(    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js\"></script>
    <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>
    <script> function makeAjaxCall(url){$.ajax({\"url\": url})}</script>
  </body>\n";
</html>)=====";