import { StyleSheet } from 'react-native';

export default StyleSheet.create({
  'body': {
    'backgroundColor': 'black',
    'minWidth': [{ 'unit': 'px', 'value': 310 }],
    'maxWidth': [{ 'unit': 'px', 'value': 1200 }],
    'height': [{ 'unit': 'px', 'value': 500 }],
    'margin': [{ 'unit': 'px', 'value': 0 }, { 'unit': 'string', 'value': 'auto' }, { 'unit': 'px', 'value': 0 }, { 'unit': 'string', 'value': 'auto' }]
  },
  'h1': {
    'color': '#e8ad54',
    'marginLeft': [{ 'unit': 'px', 'value': 20 }],
    'textSize': '0.5em'
  },
  'h2': {
    'fontFamily': 'Arial',
    'fontSize': [{ 'unit': 'rem', 'value': 2.5 }],
    'textAlign': 'center',
    'color': 'orange'
  },
  'content': {
    'color': 'blue'
  },
  'label': {
    'display': 'block',
    'padding': [{ 'unit': 'px', 'value': 8 }, { 'unit': 'px', 'value': 22 }, { 'unit': 'px', 'value': 8 }, { 'unit': 'px', 'value': 22 }],
    'margin': [{ 'unit': 'px', 'value': 0 }, { 'unit': 'px', 'value': 0 }, { 'unit': 'px', 'value': 1 }, { 'unit': 'px', 'value': 0 }],
    'cursor': 'pointer',
    'background': '#ff8000',
    'borderRadius': '3px',
    'color': '#FFF',
    'transition': 'ease .5s'
  },
  'label:hover': {
    'background': '#ffad5c'
  },
  'content': {
    'background': '#ffe6cc',
    'padding': [{ 'unit': 'px', 'value': 10 }, { 'unit': 'px', 'value': 25 }, { 'unit': 'px', 'value': 10 }, { 'unit': 'px', 'value': 25 }],
    'border': [{ 'unit': 'px', 'value': 1 }, { 'unit': 'string', 'value': 'solid' }, { 'unit': 'string', 'value': '#A7A7A7' }],
    'margin': [{ 'unit': 'px', 'value': 0 }, { 'unit': 'px', 'value': 0 }, { 'unit': 'px', 'value': 1 }, { 'unit': 'px', 'value': 0 }],
    'borderRadius': '3px'
  },
  'input + label + content': {
    'display': 'none'
  },
  'input:checked + label + content': {
    'display': 'block'
  },
  'accordeon-checkbox': {
    'display': 'none'
  }
});
