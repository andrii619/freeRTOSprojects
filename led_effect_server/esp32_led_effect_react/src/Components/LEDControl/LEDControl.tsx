import React, { useEffect, useState } from 'react';
import axios from "axios";
import { LEDState, LEDMode } from './types';
import styles from './LEDControl.module.css';


const LEDControl: React.FC = () => {
  const [currentMode, setCurrentMode] = useState<LEDMode>('ALL_OFF');

  useEffect(() => {
    const fetchMode = async () => {
      const response = await fetch('http://192.168.50.145:5000/led-effect');
      console.log("Got response: ", response)
      const data: LEDState = await response.json();
      setCurrentMode(data.currentLEDMode as LEDMode);
    };
    fetchMode();
  }, []);

  const handleModeChange = async (newMode: LEDMode) => {
    setCurrentMode(newMode);
    // await fetch('http://192.168.168.145:5000/led-effect', {
    //   method: 'POST',
    //   //headers: { 'Content-Type': 'application/json' },
    //   body: JSON.stringify({id: 1, currentLEDMode: newMode } as LEDState)
    // });
    const currentData:LEDState = {id: 1, currentLEDMode: newMode } as LEDState;
    const postHeaders = {
      'Content-Type': 'application/json',
      'Accept': '*/*',
      'Connection': 'keep-alive'
    }
    axios.post("http://192.168.50.145:5000/led-effect", currentData, {headers: postHeaders}).then((response) => {
      console.log(response.status, response.data.token);
    });
  };

  const renderLEDs = () => {
    switch (currentMode) {
      case 'ALL_OFF': {
        return (<div>
          <div className={`${styles.LEDPill} ${styles.offLED}`}>meow</div>
          <div className={`${styles.LEDPill} ${styles.offLED}`}></div>
          <div className={`${styles.LEDPill} ${styles.offLED}`}></div>
        </div>);
      }
      case 'BLINK_ALL':
        return (<div>
          <div className={`${styles.LEDPill} ${styles.redLED}   ${styles.ledAnimation}`}>meow</div>
          <div className={`${styles.LEDPill} ${styles.blueLED}  ${styles.ledAnimation}`}></div>
          <div className={`${styles.LEDPill} ${styles.greenLED} ${styles.ledAnimation}`}></div>
        </div>);
      case 'BLINK_EVEN': {
        return (<div>
          <div className={`${styles.LEDPill} ${styles.redLED}   ${styles.ledAnimation}`}></div>
          <div className={`${styles.LEDPill} ${styles.offLED}   `}></div>
          <div className={`${styles.LEDPill} ${styles.greenLED} ${styles.ledAnimation}`}></div>
        </div>);
      }
      case 'BLINK_ODD': {
        return (<div>
          <div className={`${styles.LEDPill} ${styles.offLED}`}></div>
          <div className={`${styles.LEDPill} ${styles.blueLED} ${styles.ledAnimation}`}></div>
          <div className={`${styles.LEDPill} ${styles.offLED}`}></div>
        </div>);
      }
      case 'ALTERNATE': {
        return (<div>
          <div className={`${styles.LEDPill} ${styles.redLED}`}></div>
          <div className={`${styles.LEDPill} ${styles.blueLED}`}></div>
          <div className={`${styles.LEDPill} ${styles.greenLED}`}></div>
        </div>);
      }
      default:
        return (<div>
          <div className={`${styles.LEDPill} ${styles.offLED}`}>meow</div>
          <div className={`${styles.LEDPill} ${styles.offLED}`}></div>
          <div className={`${styles.LEDPill} ${styles.offLED}`}></div>
        </div>);
    }
  };

  return (
    <div className={styles.container}>
      <select
        value={currentMode}
        onChange={(e) => handleModeChange(e.target.value as LEDMode)}
        className={styles.dropdown}
      >
        <option value="ALL_OFF">All LEDs Off</option>
        <option value="BLINK_ALL">Blink All LEDs</option>
        <option value="BLINK_EVEN">Blink Even LEDs</option>
        <option value="BLINK_ODD">Blink Odd LEDs</option>
        <option value="ALTERNATE">Alternate Blinking</option>
      </select>
      {renderLEDs()}
    </div>
  );
};

export default LEDControl;
