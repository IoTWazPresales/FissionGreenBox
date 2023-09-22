import { FC, useState } from 'react';
import {
  Typography,
  Paper,
  Stack,
  Alert
} from '@mui/material';

import { SectionContent } from '../components';
import { updateValue, useWs } from '../utils';
import { WEB_SOCKET_ROOT } from '../api/endpoints';
import {
  LightState,
  MoistureState,
  PHState,
  RTCState,
  S_H_DHT11State,
  TDSState,
  TemperatureState,
  RelayState
} from './types';

import { Widget } from '@iotnxt-dev/reactcomponentsmaterialui/build/src/components/widget';

export const LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "lightState";

const dataMoisture = [
  65, 65, 64.5, 64, 63, 62, 60, 57, 54, 50, 45, 41, 38, 36, 34, 31, 28, 24, 19
];

const dataLights = [
  1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1
];

const dataAirPump = [
  1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
];

const dataFanRelay = [
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
];

const dataPowerConsumation = [
  534, 521, 540, 240, 220, 235, 238, 228
];

const Dashboard: FC = () => {
  // LIGHT
  const [onBoardLedHistory, setOnBoardLedHistory] = useState([0, 0, 0, 0, 0, 0, 0, 0, 1]);

  const LightStateData = useWs<LightState>(LIGHT_SETTINGS_WEBSOCKET_URL);

  const setOnboardLED = (e: any) => {
    onBoardLedHistory.shift();
    onBoardLedHistory.push(e.target.checked ? 1 : 0);
    setOnBoardLedHistory(onBoardLedHistory);
    const a = updateValue(LightStateData.updateData);
    a(e);
  };

  ///// TEMPERATURE
  const TemperatureStateData = useWs<TemperatureState>(WEB_SOCKET_ROOT + "TemperatureState");
  const s_H_DHT11State = useWs<S_H_DHT11State>(WEB_SOCKET_ROOT + "s_H_DHT11State");
  const MoistureStateData = useWs<MoistureState>(WEB_SOCKET_ROOT + "MoistureState");
  const TDSStateData = useWs<TDSState>(WEB_SOCKET_ROOT + "TDSState");
  const PHStateData = useWs<PHState>(WEB_SOCKET_ROOT + "PHState");
  const RTCStateData = useWs<RTCState>(WEB_SOCKET_ROOT + "RTCState");
  const RelayStateData = useWs<RelayState>(WEB_SOCKET_ROOT + "RelayState");

  const setRelay = (e: any) => {
    const a = updateValue(RelayStateData.updateData);
    a(e);
  };

  // SMART FILM
  const [SmartFilmHistory, setSmartFilmHistory] = useState([1, 0, 0, 0, 1, 0, 0, 0, 0]);
  const [SmartFilmState, setSmartFilmState] = useState(false);
  const setSmartFilm = (e: any) => {
    SmartFilmHistory.shift();
    SmartFilmHistory.push(e.target.checked ? 1 : 0);
    setSmartFilmHistory(SmartFilmHistory);
    setSmartFilmState(e.target.checked);
  };

  return (
    <SectionContent title='GrowBox Dashboard' titleGutter>
      <Typography variant="body1" paragraph>
        System status and direct control panel.
      </Typography>

      <Alert severity='error' >
        Soil moisture too low, please switch on water pump.
      </Alert>

      <Paper
        elevation={20}
        sx={{
          p: 0.5,
          display: 'flex',
          flexDirection: 'row',
          mt: 1
        }}
      >
        <Stack spacing={0} direction="row" flexWrap='wrap'>

          <Widget
            name="led_on"
            key={`onboardled_${LightStateData.data?.led_on}`}
            label="Onboard LED"
            data={onBoardLedHistory}
            on={LightStateData.data?.led_on}
            busy={(!LightStateData.connected || !LightStateData.data)}
            onChange={setOnboardLED}
            type='boolean'
          />

          {/* DS18B20 */}
          <Widget
            name="watertemperature"
            key={`watertemperature_${TemperatureStateData.data?.watertemperature}`}
            label="Water Temperature"
            data={[0, 0, 0, 0, 0, 0, 0, 0, 1]}
            type='number'
            value={TemperatureStateData.data?.watertemperature}
            busy={(!TemperatureStateData.connected || !TemperatureStateData.data)}
            unit="°C"
            message=' '
          />

          {/* DHT11 */}
          <Widget
            name="airtemperature"
            key={`airtemperature_${s_H_DHT11State.data?.airtemperature}`}
            label="Air Temperature"
            data={[0, 0, 0, 0, 0, 0, 0, 0, 1]}
            type='number'
            value={s_H_DHT11State.data?.airtemperature}
            busy={(!s_H_DHT11State.connected || !s_H_DHT11State.data)}
            unit="°C"
            message=' '
          />

          <Widget
            name="airhumidity"
            key={`airhumidity_${s_H_DHT11State.data?.airhumidity}`}
            label="Air Humidity"
            data={[0, 0, 0, 0, 0, 0, 0, 0, 1]}
            type='number'
            value={s_H_DHT11State.data?.airhumidity}
            busy={(!s_H_DHT11State.connected || !s_H_DHT11State.data)}
            unit="%RH"
            message=' '
          />

          <Widget
            name="moisture"
            label="Soil Moisture"
            data={[0, 0, 0, 0, 0, 0, 0, 0, 1]}
            type='number'
            thresholds={[
              {
                min: -1, max: 25, level: 'warning',
                message: 'soil is too dry',
                color: 'error'
              },
              {
                min: 25, max: 75, level: 'info',
                message: 'good soil moisture',
                color: 'primary'
              },
              {
                min: 75, max: 101, level: 'warning',
                message: 'soil is too wet',
                color: 'error'
              }
            ]}
            value={MoistureStateData.data?.moisture}
            busy={(!MoistureStateData.connected || !MoistureStateData.data)}
            unit="%"
          />

          <Widget
            name="tds"
            label="Water Particulates"
            data={[0, 0, 0, 0, 0, 0, 0, 0, 1]}
            type='number'
            thresholds={[
              {
                min: -1, max: 10, level: 'info',
                message: 'water is clean',
                color: 'primary'
              },
              {
                min: 10, max: 25, level: 'warning',
                message: 'water is okay',
                color: 'secondary'
              },
              {
                min: 25, max: 101, level: 'warning',
                message: 'water is dirty',
                color: 'error'
              }
            ]}
            value={TDSStateData.data?.tds}
            busy={(!TDSStateData.connected || !TDSStateData.data)}
            unit="%"
          />

          <Widget
            name="ph"
            label="Water PH"
            data={[0, 0, 7, 7, 7]}
            type='number'
            value={PHStateData.data?.ph}
            busy={(!PHStateData.connected || !PHStateData.data)}
            unit="pH"
          />

          <Widget
            name="timestamp"
            label="RTC Timestamp"
            type='string'
            value={RTCStateData.data?.timestamp.replace("T", " ")}
            message={" "}
          />

          {/* PLACEHOLDERS */}

          <Widget
            name="ch1"
            key={`relay1_${RelayStateData.data?.ch1}`}
            label="Relay Ch1"
            on={RelayStateData.data?.ch1}
            busy={(!RelayStateData.connected || !RelayStateData.data)}
            onChange={setRelay}
            type='boolean'
          />

          <Widget
            name="ch2"
            key={`relay2_${RelayStateData.data?.ch2}`}
            label="Relay Ch2"
            on={RelayStateData.data?.ch2}
            busy={(!RelayStateData.connected || !RelayStateData.data)}
            onChange={setRelay}
            type='boolean'
          />

          <Widget
            name="ch3"
            key={`relay3_${RelayStateData.data?.ch3}`}
            label="Relay Ch3"
            on={RelayStateData.data?.ch3}
            busy={(!RelayStateData.connected || !RelayStateData.data)}
            onChange={setRelay}
            type='boolean'
          />

          <Widget
            name="ch4"
            key={`relay4_${RelayStateData.data?.ch4}`}
            label="Relay Ch4"
            on={RelayStateData.data?.ch4}
            busy={(!RelayStateData.connected || !RelayStateData.data)}
            onChange={setRelay}
            type='boolean'
          />

          <Widget
            label="Smart Film"
            value={false}
            type='boolean'
            data={[0, 1, 2, 3, 4, 3, 2, 1, 2]}
          />

          <Widget
            label="Power Consumption"
            data={dataPowerConsumation}
            type='number'
            unit='Watt'
            value={234}
            message='168.9 kwh (30 days)'
          />

          <Widget
            label="Lights"
            data={dataLights}
            on
            schedule={{
              on: '7:00 am',
              off: '21:00 pm'
            }}
            type='boolean'
          />

          <Widget
            label="Air Pump"
            data={dataAirPump}
            on={false}
            type='boolean'
          />

          <Widget
            label="Extractor Fan"
            data={dataFanRelay}
            on={true}
            type='boolean'
          />

        </Stack>
      </Paper>

    </SectionContent>

  );
};

export default Dashboard;
