
import React, { FC } from 'react';
import { Navigate, Route, Routes } from 'react-router-dom';

import { Tab } from '@mui/material';

import { RouterTabs, useRouterTab, useLayoutTitle } from '../components';

import DemoInformation from './DemoInformation';
import LightStateRestForm from './LightStateRestForm';
import LightMqttSettingsForm from './LightMqttSettingsForm';
import LightStateWebSocketForm from './LightStateWebSocketForm';
import Dashboard from './Dashboard';

const GrowBox: FC = () => {
  useLayoutTitle("GrowBox");
  const { routerTab } = useRouterTab();

  return (
    <>
      <RouterTabs value={routerTab}>
        <Tab value="dashboard" label="Dashboard" />
        <Tab value="information" label="Schedules" />
        <Tab value="rest" label="Presets" />
        <Tab value="socket" label="History" />
        <Tab value="mqtt" label="Settings" />
      </RouterTabs>
      <Routes>
        <Route path="information" element={<DemoInformation />} />
        <Route path="rest" element={<LightStateRestForm />} />
        <Route path="mqtt" element={<LightMqttSettingsForm />} />
        <Route path="socket" element={<LightStateWebSocketForm />} />
        <Route path="dashboard" element={<Dashboard />} />
        <Route path="/*" element={<Navigate replace to="dashboard" />} />
      </Routes>
    </>
  );
};

export default GrowBox;
