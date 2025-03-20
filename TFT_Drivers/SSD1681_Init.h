// SSD1681 initialization sequence

{
#ifdef TFT_BUSY
    pinMode(TFT_BUSY, INPUT);
#endif

    // Hardware reset
    digitalWrite(TFT_RST, LOW);
    delay(10);
    digitalWrite(TFT_RST, HIGH);
    delay(10);

    // Wait for busy signal
    CHECK_BUSY();
    writecommand(0x12); // Software reset
    CHECK_BUSY();

    // Set driver output control
    writecommand(0x01);
    writedata((EPD_HEIGHT - 1) % 256); // 199 % 256 = 199
    writedata((EPD_HEIGHT - 1) / 256); // 199 / 256 = 0
    writedata(0x00);

    // Set data entry mode
    writecommand(0x11);
    writedata(0x01);

    // Set RAM X address range
    writecommand(0x44);
    writedata(0x00);
    writedata(EPD_WIDTH / 8 - 1); // 200 / 8 - 1 = 24

    // Set RAM Y address range
    writecommand(0x45);
    writedata((EPD_HEIGHT - 1) % 256); // 199 % 256 = 199
    writedata((EPD_HEIGHT - 1) / 256); // 199 / 256 = 0
    writedata(0x00);
    writedata(0x00);

    // Set border waveform
    writecommand(0x3C);
    writedata(0x05);

    // Read built-in temperature sensor
    writecommand(0x18);
    writedata(0x80);

    // Set RAM address counter
    writecommand(0x4E);
    writedata(0x00);
    writecommand(0x4F);
    writedata((EPD_HEIGHT - 1) % 256); // 199 % 256 = 199
    writedata((EPD_HEIGHT - 1) / 256); // 199 / 256 = 0
    CHECK_BUSY();
}
