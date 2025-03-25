
# ⚡ CO2Calculator 
A tool for visualizing electricity generation emissions based on ENTSO-E data.

---

## Getting Started - Release 1.0.0
You can try the release. The functionality is still very rudementary. If you want to try nonetheless follow the steps below.

1. Download and extract the ZIP archive.
2. Copy `config.json.example` and rename it to `config.json`.
3. Enter your **ENTSO-E API key** (see below).
4. Launch `CO2Calculator_Frontend.exe`.

---

## 🔑 How to Get an ENTSO-E API Key

1. Visit: https://www.entsoe.eu
2. Create a free user account.
3. Under your account, request an API key for the ENTSO-E Transparency Platform.
4. The key will be sent to you via email.

💡 Activation may take 1–2 business days.

---

## 🧾 Sample `config.json`

```json
{
  "api_key": "YOUR_API_KEY_HERE",
  "base_url": "https://web-api.tp.entsoe.eu/api"
}
```

📁 This file must be placed in the **same directory** as the `.exe` file.
