void html_answer(String stringName_HTML, int iAnzahl_HTML, String stringOrt_HTML, String stringSchrank_HTML) {
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println();

client.println("<!DOCTYPE html>");
client.println("<html lang=\"de\">");
client.println("<head>");
  client.println("<meta charset=\"UTF-8\">");
  client.println("<title>Inventarsystem</title>");
  client.println("<h1>Inventarsystem</h1>");
client.println("</head>");
client.println("<body>");
  client.println("<h2>Eintrag hinzufügen</h2>");
  client.println("<form id=\"meinFormular\">");
    client.println("<label for=\"name\">Name:</label>");
    client.println("<input type=\"text\" id=\"name\" name=\"name\"><br><br>");

    client.println("<label for=\"anzahl\">Anzahl:</label>");
    client.println("<input type=\"number\" id=\"anzahl\" name=\"anzahl\"><br><br>");

    client.println("<label for=\"ort\">Ort:</label>");
    client.println("<input type=\"text\" id=\"ort\" name=\"ort\"><br><br>");

    client.println("<label for=\"schrank\">Schrank:</label>");
    client.println("<input type=\"text\" id=\"schrank\" name=\"schrank\"><br><br>");

    client.println("<button type=\"button\" onclick=\"updateURL()\">Hinzufügen</button>");
  client.println("</form>");

  client.println("<script>");
    client.println("function updateURL() {");
      client.println("const name = document.getElementById(\"name\").value;");
      client.println("const anzahl = document.getElementById(\"anzahl\").value;");
      client.println("const ort = document.getElementById(\"ort\").value;");
      client.println("const schrank = document.getElementById(\"schrank\").value;");

      client.println("const params = new URLSearchParams({");
        client.println("name: name,");
        client.println("anzahl: anzahl,");
        client.println("ort: ort,");
        client.println("schrank: schrank");
      client.println("});");

      client.println("window.location.search = params.toString();");
    client.println("}");

    // Prüfen, ob Parameter vorhanden sind
    client.println("if (window.location.search) {");
      // Entferne die Query-Parameter (alles ab ?)
      client.println("const urlOhneParams = window.location.origin + window.location.pathname;");
      
      // Ersetze die aktuelle URL im Verlauf
      client.println("history.replaceState(null, \"\", urlOhneParams);");
    client.println("}");
  client.println("</script>");
client.println("</body>");
client.println("</html>");
client.println();
}