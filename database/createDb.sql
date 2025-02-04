CREATE TABLE actualData (
    Timestamp TEXT NOT NULL,
    Country TEXT NOT NULL DEFAULT 'Germany',
    GenerationType TEXT NOT NULL,
    Generation_MW REAL NOT NULL,
    CO2Emission_kwhg REAL NOT NULL,
    PRIMARY KEY (Timestamp, GenerationType)
);