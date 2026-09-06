void html_answer(int iCase) {

  if (iCase == 100) {
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

  client.println("<style>");
  client.print("body {background-color: "); client.print("#D8D8D8"); client.println(";}");
  client.println("h2 {background-color:#008184; color:#FFFFFF; text-align:center; font-size:40px;}");
  client.println(".gauge {position: relative; border-radius: 50%/100% 100% 0 0; background-color: var(--color, #a22); overflow: hidden;}");
  client.println(".gauge:before {content: \"\"; display: block; padding-top: 50%;}");
  client.println(".gauge .chart {overflow: hidden;}");
  client.print(".gauge .mask {position: absolute; left: 20%; right: 20%; bottom: 0; top: 40%; background-color: "); client.print("#D8D8D8"); client.println("; border-radius: 50%/100% 100% 0 0;}");
  client.println(".gauge .percentage {position:  absolute; top: -1px; left: -1px; bottom: 0; right: -1px; background-color: var(--background, #aaa); transform:rotate(var(--rotation)); transform-origin: bottom center; transition-duration: 600;}");
  client.println(".gauge:hover {--rotation: 100deg;}");
  client.println(".gauge .value {position:absolute; bottom:0%; left:0; width:100%; text-align: center;}");
  client.println(".gauge .min {position:absolute; bottom:0; left:5%;}");
  client.println(".gauge .max {position:absolute; bottom:0; right:5%;}");
  client.println("</style>");

  client.println("<title>Arduino WIFI-Hotspot</title>");
  client.println("</head>");
	
  client.println("<body>");
  client.println("<h2>Arduino WIFI-Hotspot<br>");
  client.print("SSID: "); client.print(charSSID); client.println("</h2>");

  client.println("<div class=\"flex-container\" style=\"display:inline-flex; flex-direction:row; flex-wrap:wrap; justify-content:space-around; min-width:100%;\">");

  client.print("<div class=\"gauge\" style=\"width: 400px; --rotation:"); client.print(180*fTemp/50); client.println("deg; --color:#5cb85c; --background:#FFFFFF;\">");
  client.println("<div class=\"percentage\"></div>");
  client.println("<div class=\"mask\"></div>");
  client.print("<span class=\"value\" style=\"font-size:30px;\">Temperatur:<br>"); client.print(fTemp); client.println("°C</span>");
  client.println("</div>");

  client.print("<div class=\"gauge\" style=\"width: 400px; --rotation:"); client.print(180*fHum/100); client.println("deg; --color:#5cb85c; --background:#FFFFFF;\">");
  client.println("<div class=\"percentage\"></div>");
  client.println("<div class=\"mask\"></div>");
  client.print("<span class=\"value\" style=\"font-size:30px;\">Luftfeuchtigkeit:<br>"); client.print(fHum); client.println("%</span>");
  client.println("</div>");

  client.print("<div class=\"gauge\" style=\"width: 400px; --rotation:"); client.print(180*fMoist/100); client.println("deg; --color:#5cb85c; --background:#FFFFFF;\">");
  client.println("<div class=\"percentage\"></div>");
  client.println("<div class=\"mask\"></div>");
  client.print("<span class=\"value\" style=\"font-size:30px;\">Bodenfeuchte:<br>"); client.print(fMoist); client.println("%</span>");
  client.println("</div>");

  client.println("</div>");
  client.println("</body>");
  client.println("</html>");

  // The HTTP response ends with another blank line:
  client.println();
  client.println();
  // break out of the while loop:
  }
}