
import { FC } from 'react';

import { Box, Divider, Drawer, Toolbar, Typography, styled } from '@mui/material';

import { PROJECT_NAME } from '../../api/env';
import LayoutMenu from './LayoutMenu';
import { DRAWER_WIDTH } from './Layout';

const LayoutDrawerLogo = styled('img')(({ theme }) => ({
  [theme.breakpoints.down("sm")]: {
    height: 24,
    marginRight: theme.spacing(2)
  },
  [theme.breakpoints.up("sm")]: {
    height: 36,
    marginRight: theme.spacing(2)
  }
}));

interface LayoutDrawerProps {
  mobileOpen: boolean;
  onClose: () => void;
}

const LayoutDrawer: FC<LayoutDrawerProps> = ({ mobileOpen, onClose }) => {

  const drawer = (
    <>
      <Toolbar disableGutters>
        <Box display="flex" alignItems="center" px={2}>
          {/* <LayoutDrawerLogo src="/app/icon.png" alt={PROJECT_NAME} /> */}
          <Typography variant="h6" color="textPrimary">
            {PROJECT_NAME}
          </Typography>
        </Box>
        <Divider absolute />
      </Toolbar>
      <Divider />
      <LayoutMenu />
      <Divider />
      
      <div
          style={{
            display: 'flex',
            flexDirection: 'column',
            width: '100%',
            padding: 5,
            flexWrap:'wrap',
            alignItems: 'center',
            justifyContent: 'center'
          }}
        >
          <img style={{ padding: 5, height: 60 }} src="/app/logo_GreenDoor_Expo.png" alt="GreenDoor" />
          <img style={{ padding: 5, height: 80 }} src="/app/logo_Indorsun_White.png" alt="Indorsun" />
          <img style={{ padding: 5, height: 80 }} src="/app/logo_TheGrowBar.png" alt="GrowBar" />
          <img style={{ padding: 5, height: 60 }} src="/app/logo_iotnxt.png" alt="GreenDoor" />
        </div>

    </>
  );

  return (
    <Box
      component="nav"
      sx={{ width: { md: DRAWER_WIDTH }, flexShrink: { md: 0 } }}
    >
      <Drawer
        variant="temporary"
        open={mobileOpen}
        onClose={onClose}
        ModalProps={{
          keepMounted: true, // Better open performance on mobile.
        }}
        sx={{
          display: { xs: 'block', md: 'none' },
          '& .MuiDrawer-paper': { boxSizing: 'border-box', width: DRAWER_WIDTH },
        }}
      >
        {drawer}
      </Drawer>
      <Drawer
        variant="permanent"
        sx={{
          display: { xs: 'none', md: 'block' },
          '& .MuiDrawer-paper': { boxSizing: 'border-box', width: DRAWER_WIDTH },
        }}
        open
      >
        {drawer}
      </Drawer>
    </Box>
  );

};

export default LayoutDrawer;
