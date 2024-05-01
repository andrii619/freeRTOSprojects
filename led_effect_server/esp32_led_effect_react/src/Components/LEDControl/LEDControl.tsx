import React, { useEffect, useState } from 'react';
import { LEDState, LEDMode } from './types';
import styles from './LEDControl.module.css';


const LEDControl: React.FC = () => {
  const [currentMode, setCurrentMode] = useState<LEDMode>('ALL_OFF');

  useEffect(() => {
    const fetchMode = async () => {
      const response = await fetch('http://192.168.50.144/led-effect');
      const data: LEDState = await response.json();
      setCurrentMode(data.currentLEDMode as LEDMode);
    };
    fetchMode();
  }, []);

  const handleModeChange = async (newMode: LEDMode) => {
    setCurrentMode(newMode);
    await fetch('http://192.168.168.144/led-effect/', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({id: 1, currentLEDMode: newMode } as LEDState)
    });
  };

  const renderLEDs = () => {
    switch (currentMode) {
      case 'ALL_OFF': {
        return (<div>
          <div className={`${styles.led}`}>meow</div>
          <div className={`${styles.led}`}></div>
          <div className={`${styles.led}`}></div>
        </div>);
      }
      case 'BLINK_ALL':
        return (<div>
          <div className={`${styles.led} ${styles.animationRedLED}`}>meow</div>
          <div className={`${styles.led} ${styles.animationBlueLED}`}></div>
          <div className={`${styles.led} ${styles.animationGreenLED}`}></div>
        </div>);
      case 'BLINK_EVEN': {
        return (<div>
          <div className={`${styles.led} ${styles.animationRedLED}`}></div>
          <div className={`${styles.led}`}></div>
          <div className={`${styles.led} ${styles.animationGreenLED}`}></div>
        </div>);
      }
      case 'BLINK_ODD': {
        return (<div>
          <div className={`${styles.led}`}></div>
          <div className={`${styles.led} ${styles.animationBlueLED}`}></div>
          <div className={`${styles.led}`}></div>
        </div>);
      }
      // case 'ALTERNATE':
      //   return (
      //     <div>
      //       <div className={`${styles.animation}`} style={{animation: 'alternate 2s infinite'}}></div>
      //       <div className={`${styles.animation}`} style={{animation: 'alternate 2s infinite', animationDelay: '1s'}}></div>
      //       <div className={`${styles.animation}`} style={{animation: 'alternate 2s infinite'}}></div>
      //     </div>
      //   );
      default:
        return (<div>
          <div className={`${styles.led}`}>meow</div>
          <div className={`${styles.led}`}></div>
          <div className={`${styles.led}`}></div>
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
