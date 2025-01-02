// Пины для подрулевых лепестков
const int paddleLeft = 4;  // Левый лепесток
const int paddleRight = 5; // Правый лепесток

// Пины для кнопок
const int buttonPins[] = {12, 13, 14, 15, 16, 17, 18, 19};
const int numButtons = 8;

// Пины для энкодеров KY-040
const int encoder1CLK = 22; // Сигнал A энкодера 1
const int encoder1DT = 23;  // Сигнал B энкодера 1
const int encoder1SW = 25;  // Кнопка энкодера 1

const int encoder2CLK = 26; // Сигнал A энкодера 2
const int encoder2DT = 27;  // Сигнал B энкодера 2
const int encoder2SW = 32;  // Кнопка энкодера 2

// Переменные для отслеживания положения энкодеров
int encoder1Position = 0;
int encoder2Position = 0;
int lastStateCLK1;
int lastStateCLK2;

// Переменные для отслеживания состояния кнопок и дребезга
unsigned long lastDebounceTime[numButtons + 2] = {0}; // +2 для лепестков
bool buttonState[numButtons + 2] = {false};          // Состояние кнопок
bool lastButtonState[numButtons + 2] = {true};       // Предыдущее состояние кнопок
const unsigned long debounceDelay = 50;             // Задержка debounce

void setup() {
  Serial.begin(115200);

  // Инициализация кнопок
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Инициализация пинов энкодеров
  pinMode(encoder1CLK, INPUT);
  pinMode(encoder1DT, INPUT);
  pinMode(encoder1SW, INPUT_PULLUP);
  pinMode(encoder2CLK, INPUT);
  pinMode(encoder2DT, INPUT);
  pinMode(encoder2SW, INPUT_PULLUP);

  // Инициализация подрулевых лепестков
  pinMode(paddleLeft, INPUT_PULLUP);
  pinMode(paddleRight, INPUT_PULLUP);

  lastStateCLK1 = digitalRead(encoder1CLK);
  lastStateCLK2 = digitalRead(encoder2CLK);
}

void loop() {
  unsigned long currentTime = millis();

  // Проверка состояния кнопок с debounce
  for (int i = 0; i < numButtons; i++) {
    bool currentButtonState = digitalRead(buttonPins[i]) == LOW;
    if (currentButtonState != lastButtonState[i]) {
      lastDebounceTime[i] = currentTime;
    }
    if ((currentTime - lastDebounceTime[i]) > debounceDelay) {
      if (currentButtonState != buttonState[i]) {
        buttonState[i] = currentButtonState;
        if (buttonState[i]) {
          Serial.print("Button ");
          Serial.print(i + 1);
          Serial.println(" pressed");
        }
      }
    }
    lastButtonState[i] = currentButtonState;
  }

  // Обработка подрулевых лепестков
  bool currentPaddleStateLeft = digitalRead(paddleLeft) == LOW;
  if (currentPaddleStateLeft != lastButtonState[numButtons]) {
    lastDebounceTime[numButtons] = currentTime;
  }
  if ((currentTime - lastDebounceTime[numButtons]) > debounceDelay) {
    if (currentPaddleStateLeft != buttonState[numButtons]) {
      buttonState[numButtons] = currentPaddleStateLeft;
      if (buttonState[numButtons]) {
        Serial.println("Left paddle pressed");
      }
    }
  }
  lastButtonState[numButtons] = currentPaddleStateLeft;

  bool currentPaddleStateRight = digitalRead(paddleRight) == LOW;
  if (currentPaddleStateRight != lastButtonState[numButtons + 1]) {
    lastDebounceTime[numButtons + 1] = currentTime;
  }
  if ((currentTime - lastDebounceTime[numButtons + 1]) > debounceDelay) {
    if (currentPaddleStateRight != buttonState[numButtons + 1]) {
      buttonState[numButtons + 1] = currentPaddleStateRight;
      if (buttonState[numButtons + 1]) {
        Serial.println("Right paddle pressed");
      }
    }
  }
  lastButtonState[numButtons + 1] = currentPaddleStateRight;

  // Обработка энкодеров
  int currentStateCLK1 = digitalRead(encoder1CLK);
  if (currentStateCLK1 != lastStateCLK1) {
    if (digitalRead(encoder1DT) != currentStateCLK1) {
      encoder1Position++;
    } else {
      encoder1Position--;
    }
    Serial.print("Encoder 1 Position: ");
    Serial.println(encoder1Position);
  }
  lastStateCLK1 = currentStateCLK1;

  if (digitalRead(encoder1SW) == LOW && (currentTime - lastDebounceTime[numButtons]) > debounceDelay) {
    lastDebounceTime[numButtons] = currentTime;
    Serial.println("Encoder 1 button pressed");
  }

  int currentStateCLK2 = digitalRead(encoder2CLK);
  if (currentStateCLK2 != lastStateCLK2) {
    if (digitalRead(encoder2DT) != currentStateCLK2) {
      encoder2Position++;
    } else {
      encoder2Position--;
    }
    Serial.print("Encoder 2 Position: ");
    Serial.println(encoder2Position);
  }
  lastStateCLK2 = currentStateCLK2;

  if (digitalRead(encoder2SW) == LOW && (currentTime - lastDebounceTime[numButtons + 1]) > debounceDelay) {
    lastDebounceTime[numButtons + 1] = currentTime;
    Serial.println("Encoder 2 button pressed");
  }
}
