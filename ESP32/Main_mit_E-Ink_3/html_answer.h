#define body_background_color "#D8D8D8"
#define h2_background_color "#008184"
#define h2_color "#FFFFFF"
#define gauge_background_color "#FFFFFF"
#define gauge_color "#5cb85c"
#define chart_background_color "white"
#define chart_border_width "2px"
#define chart_border_color "#555555"


void html_answer(int iCase) {
  
  if (iCase == 100) {
    int iChart_length_klein = 0;
    for (int i=0; i <= Chart_length; i++) {if (iMoist_Array[i] != -1) {iChart_length_klein++;}}
    Serial.print("iChart_length_klein = ");
    Serial.println(iChart_length_klein);
    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
    // and a content-type so the client knows what's coming, then a blank line:
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    // the content of the HTTP response follows the header:
    client.println("<!DOCTYPE html>");
    client.println("<html lang=\"de\">");
    client.println("<head>");
    client.println("<meta charSet=\"UTF-8\"/>");
    client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
    client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
    client.print  ("<link  rel='shortcut icon' type='image/png' href=\"");
    client.print  ("data:image/png;base64,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");
    client.println("\">"); //img 60x60

    client.println("<style>");
    client.print  ("body {background-color: "); client.print(body_background_color); client.println(";}");
    client.print  ("h2 {background-color: "); client.print(h2_background_color); client.print("; color:"); client.print(h2_color); client.println("; text-align:center; font-size:40px;}");
    client.println("/* ------------------------------------------ */");
    client.println(".gauge {position: relative; border-radius: 50%/100% 100% 0 0; background-color: var(--color, #a22); overflow: hidden; margin: 10px 10px 10px 10px;}");
    client.println(".gauge:before {content: \"\"; display: block; padding-top: 50%;}");
    client.println(".gauge .chart {overflow: hidden;}");
    client.print  (".gauge .mask {position: absolute; left: 20%; right: 20%; bottom: 0; top: 40%; background-color: "); client.print(body_background_color); client.println("; border-radius: 50%/100% 100% 0 0;}");
    client.println(".gauge .percentage {position:  absolute; top: -1px; left: -1px; bottom: 0; right: -1px; background-color: var(--background, #aaa); transform:rotate(var(--rotation)); transform-origin: bottom center; transition-duration: 600;}");
    client.println(".gauge:hover {--rotation: 100deg;}");
    client.println(".gauge .value {position:absolute; bottom:0%; left:0; width:100%; text-align: center;}");
    client.println(".gauge .min {position:absolute; bottom:0; left:5%;}");
    client.println(".gauge .max {position:absolute; bottom:0; right:5%;}");
    client.println("/* ------------------------------------------ */");
    client.print  (".chart {background: "); client.print(chart_background_color);
      client.print  ("; width: 100%; hight: auto; max-height: 500px; min-width: 350px; max-width: "); client.print(iChart_length_klein);
      client.print  ("px; border-left: "); client.print(chart_border_width); client.print(" dotted "); client.print(chart_border_color);
      client.print  ("; border-right: "); client.print(chart_border_width); client.print(" dotted "); client.print(chart_border_color);
      client.print  ("; border-top: "); client.print(chart_border_width); client.print(" dotted "); client.print(chart_border_color);
      client.print  ("; border-bottom: "); client.print(chart_border_width); client.print(" dotted "); client.print(chart_border_color);
      client.println("; padding: 20px 20px 20px 20px; margin: 10px 10px 10px 10px;}");
    client.println("/* ------------------------------------------ */");
    client.println(".labels {font-size: 13px;}");
    client.println("/* ------------------------------------------ */");
    client.println("</style>");

    client.println("<title>Arduino WIFI-Hotspot</title>");
    client.println("</head>");
	
    client.println("<body>");
    client.println("<h2>Arduino WIFI-Hotspot<br>");
      client.print  ("SSID: "); client.print(charSSID); client.println("</h2>");

    client.println("<div class=\"flex-container\" style=\"display:inline-flex; flex-direction:row; flex-wrap:wrap; justify-content:space-around; min-width:100%;\">");

    client.print  ("<div class=\"gauge\" style=\"width: 400px; --rotation:"); client.print(180*fTemp/50); client.print("deg; --color: "); client.print(gauge_color); client.print("; --background: "); client.print(gauge_background_color); client.println(";\">");
    client.println("<div class=\"percentage\"></div>");
    client.println("<div class=\"mask\"></div>");
    client.print  ("<span class=\"value\" style=\"font-size:30px;\">Temperatur:<br>"); client.print(fTemp); client.println("°C</span>");
    client.println("</div>");

    client.print  ("<div class=\"gauge\" style=\"width: 400px; --rotation:"); client.print(180*fHum/100); client.print("deg; --color: "); client.print(gauge_color); client.print("; --background: "); client.print(gauge_background_color); client.println(";\">");
    client.println("<div class=\"percentage\"></div>");
    client.println("<div class=\"mask\"></div>");
    client.print  ("<span class=\"value\" style=\"font-size:30px;\">Luftfeuchtigkeit:<br>"); client.print(fHum); client.println("%</span>");
    client.println("</div>");

    client.print  ("<div class=\"gauge\" style=\"width: 400px; --rotation:"); client.print(180*fMoist/100); client.print("deg; --color: "); client.print(gauge_color); client.print("; --background: "); client.print(gauge_background_color); client.println(";\">");
    client.println("<div class=\"percentage\"></div>");
    client.println("<div class=\"mask\"></div>");
    client.print  ("<span class=\"value\" style=\"font-size:30px;\">Bodenfeuchte:<br>"); client.print(fMoist); client.println("%</span>");
    client.println("</div>");

    client.println("</div>");
    client.println("<!-- ------------------------------------------ -->");

    client.println("<div class=\"flex-container\" style=\"display:inline-flex; flex-direction:row; flex-wrap:wrap; justify-content:space-around; min-width:100%;\">");

    client.print  ("<svg viewBox=\"-20 -10 "); client.print(iChart_length_klein+30); client.println(" 275\" class=\"chart\">");
    client.println("<g class=\"labels x-labels\">");
      for (int i = iChart_length_klein; i >= 0; i-=50) {client.print("<text x=\""); client.print(i); client.print("\" y=\"260\">-"); client.print(iChart_length_klein-i); client.println("</text>");}
      client.println("</g>");
    client.println("<g class=\"labels y-labels\">");
      for (int i = 0; i < iChart_length_klein+20; i+=200) {
        client.print("<text x=\""); client.print(i-20); client.println("\" y=  \"5\">120</text>");
        client.print("<text x=\""); client.print(i-20); client.println("\" y= \"45\">100</text>");
        client.print("<text x=\""); client.print(i-15); client.println("\" y= \"85\"> 80</text>");
        client.print("<text x=\""); client.print(i-15); client.println("\" y=\"125\"> 60</text>");
        client.print("<text x=\""); client.print(i-15); client.println("\" y=\"165\"> 40</text>");
        client.print("<text x=\""); client.print(i-15); client.println("\" y=\"205\"> 20</text>");
        client.print("<text x=\""); client.print(i-10); client.println("\" y=\"245\">  0</text>");
      }
      client.println("</g>");
    client.println("<polyline fill=\"none\" stroke=\"#0074d9\" stroke-width=\"2\" points=\"");
      for (int i = 0; i < iChart_length_klein; i++) {client.print(i); client.print(", "); client.println(240 - (2 * iMoist_Array[Chart_length+1-iChart_length_klein+i]));} client.println("\"/>");
  
    client.println("</svg>");
    client.println("</div>");
    client.println("<!-- ------------------------------------------ -->");
    client.println("<form action=\"/\">");
    client.println("<label for=\"Min_Moist_percent\">Mindestbodenfeuchtigkeit:</label>");
    client.print  ("<input type=\"number\" id=\"Min_Moist_percent\" name=\"iMin_Moist_percent\" value=\""); client.print(iMin_Moist_percent); client.println("\"><br><br>");
    client.println("<input type=\"submit\" value=\"Ändern\"></form>");
    client.println("<form action=\"/\">");
    client.println("<label for=\"Stop_Moist_percent\">Höchstbodenfeuchtigkeit:</label>");
    client.print  ("<input type=\"number\" id=\"Stop_Moist_percent\" name=\"iStop_Moist_percent\" value=\""); client.print(iStop_Moist_percent); client.println("\"><br><br>");
    client.println("<input type=\"submit\" value=\"Ändern\"></form>");
    client.println("<!-- ------------------------------------------ -->");
    client.println("</body>");
    client.println("</html>");

    // The HTTP response ends with another blank line:
    client.println();
    client.println();
  }
  else if (iCase == 101) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<!DOCTYPE html>");
    client.println("<html lang=\"de\">");
    client.println("<head>");
    client.println("<meta name=\"viewport\" content=\"width=device-width; height=device-height;\">");
    client.println("<title>favicon.ico</title>");
    client.print  ("<link  rel='shortcut icon' type='image/png' href=\"");
    client.print  ("data:image/png;base64,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");
    client.println("\">"); //img 60x60
    client.println("</head>");
    client.println("<body>");
    client.print  ("<img class=\"transparent\" src=\"");
    client.print  ("data:image/png;base64,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");
    client.println("\"></img>"); //img 120x120
    client.println("</body>");
    client.println("</html>");
    // The HTTP response ends with another blank line:
    client.println();
    client.println();
  }
  else if (iCase == 102) {
    client.println("HTTP/1.1 403 Forbidden");
    client.println("Content-type:text/plain");
    client.println();
    client.println("403 Forbidden");
    client.println();
    client.println();
  }
  else if (iCase == 2000) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/plain");
    client.println();
    client.println("200 OK \nMin/Stop_Moist:");
    client.println(iMin_Moist_percent+2200);
    client.println(iStop_Moist_percent+2400);
    client.println();
    client.println();
  }
  else if (iCase == 2200) {
    client.println("HTTP/1.1 202 Accepted");
    client.println("Content-type:text/plain");
    client.println();
    client.print("202 Accepted \nData updated. Min_Moist_percent now: ");
    client.println(iMin_Moist_percent);
    client.println();
    client.println();
  }
  else if (iCase == 2400) {
    client.println("HTTP/1.1 202 Accepted");
    client.println("Content-type:text/plain");
    client.println();
    client.print("202 Accepted \nData updated. Stop_Moist_percent now: ");
    client.println(iStop_Moist_percent);
    client.println();
    client.println();
  }
  else if (iCase == 1000 || iCase == 1000000 || iCase == 1000000000 || iCase == 1100000000) {
    client.println("HTTP/1.1 202 Accepted");
    client.println("Content-type:text/plain");
    client.println();
    client.println("202 Accepted (Content under construction)");
    client.println();
    client.println();
  }
  else {
    client.println("HTTP/1.1 403 Forbidden");
    client.println("Content-type:text/plain");
    client.println();
    client.println("403 Forbidden");
    // The HTTP response ends with another blank line:
    client.println();
    client.println();
  }
}