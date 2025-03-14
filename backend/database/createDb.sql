CREATE TABLE IF NOT EXISTS actualGenerationData (
    Timestamp TEXT NOT NULL,
    Country TEXT NOT NULL DEFAULT 'Germany',
    GenerationType TEXT NOT NULL,
    Generation_MW REAL NOT NULL,
    --CO2Emission_kwhg REAL NOT NULL,
    PRIMARY KEY (Timestamp, GenerationType)
);

--CREATE TABLE IF NOT EXIST CO2Data (
--    Timestamp TEXT NOT NULL,
--    Country TEXT NOT NULL,
--    GenerationType TEXT NOT NULL,
--    CO2Emissions_kwhg REAL NOT NULL,
--    PRIMARY KEY (Timestamp, GenerationType)
--);