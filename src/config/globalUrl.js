let url;
if (process.env.NODE_ENV === 'production') {
  url = window.g.REMOTE_URL
} else {
  url = window.g.LOCAL_URL
}


//station
export const getStationsUrl = url + '/station/getStations';

//device
export const getGroupsUrl = url + '/device/getGroups';
export const deviceSelectUrl = url + '/device/select';
export const getRealtimeDataUrl = url + '/device/getRealtimeData';
export const getDeviceTrackUrl = url + '/device/getDeviceTrack';
