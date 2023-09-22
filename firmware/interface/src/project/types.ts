export interface LightState {
  led_on: boolean;
}

export interface LightMqttSettings {
  unique_id: string;
  name: string;
  mqtt_path: string;
}

export interface TemperatureState {
  watertemperature: number;
}

export interface S_H_DHT11State {
  airtemperature: number;
  airhumidity: number;
}

export interface MoistureState {
  moisture: number;
}

export interface TDSState {
  tds: number;
}

export interface PHState {
  ph: number;
}

export interface RTCState {
  timestamp: string;
}

export interface RelayState {
  ch1: boolean;
  ch2: boolean;
  ch3: boolean;
  ch4: boolean;
}
