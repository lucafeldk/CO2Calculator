#include <iostream>
#include <curl/curl.h>
#include <string>

// Callback-Funktion zum Speichern der API-Antwort
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // cURL initialisieren
    curl = curl_easy_init();
    if (curl) {
        // Ziel-URL
        curl_easy_setopt(curl, CURLOPT_URL, "https://web-api.tp.entsoe.eu/api?documentType=A26&businessType=B08&out_Domain=10YGB----------A&in_Domain=10YBE----------2&periodStart=202308202200&periodEnd=202308212200");

        // HTTP-Methode GET setzen
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

        // Automatisches Folgen von Weiterleitungen aktivieren
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Standardprotokoll auf HTTPS setzen
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

        // HTTP-Header hinzufügen (z. B. Autorisierung)
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Authorization: Bearer YOUR_API_KEY"); // Ersetze YOUR_API_KEY durch deinen API-Schlüssel
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Callback-Funktion zum Speichern der Antwort setzen
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Anfrage ausführen
        res = curl_easy_perform(curl);

        // Fehlerbehandlung
        if (res != CURLE_OK) {
            std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Ausgabe der API-Antwort
            std::cout << "API Response: " << readBuffer << std::endl;
        }

        // HTTP-Header-Liste freigeben
        curl_slist_free_all(headers);

        // cURL-Ressourcen freigeben
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize cURL!" << std::endl;
    }

    return 0;
}