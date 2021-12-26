import { StyleSheet } from 'react-native';

export default StyleSheet.create({
  'body': {
    'backgroundColor': 'lightblue',
    'minWidth': [{ 'unit': 'px', 'value': 310 }],
    'maxWidth': [{ 'unit': 'px', 'value': 800 }],
    'height': [{ 'unit': 'px', 'value': 400 }],
    'margin': [{ 'unit': 'px', 'value': 0 }, { 'unit': 'string', 'value': 'auto' }, { 'unit': 'px', 'value': 0 }, { 'unit': 'string', 'value': 'auto' }]
  },
  'h1': {
    'color': 'navy',
    'marginLeft': [{ 'unit': 'px', 'value': 20 }]
  },
  'h2': {
    'fontFamily': 'Arial',
    'fontSize': [{ 'unit': 'rem', 'value': 2.5 }],
    'textAlign': 'center'
  },
  'content': {
    'color': 'blue'
  }
});
