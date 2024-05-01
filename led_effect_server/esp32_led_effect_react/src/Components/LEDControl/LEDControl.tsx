import React, { useEffect, useState } from 'react';
import styles from './LEDControl.module.css';

type LEDMode = 'ALL_OFF' | 'BLINK_ALL' | 'BLINK_EVEN' | 'BLINK_ODD' | 'ALTERNATE';

const LEDControl: React.FC = () => {
  const [currentMode, setCurrentMode] = useState<LEDMode>('ALL_OFF');

//   useEffect(() => {
//     const fetchMode = async () => {
//       const response = await fetch('https://api.example.com/led/mode');
//       const data = await response.json();
//       setCurrentMode(data.mode as LEDMode);
//     };
//     fetchMode();
//   }, []);

  const handleModeChange = async (newMode: LEDMode) => {
    setCurrentMode(newMode);
    // await fetch('https://api.example.com/led/mode', {
    //   method: 'POST',
    //   headers: { 'Content-Type': 'application/json' },
    //   body: JSON.stringify({ mode: newMode })
    // });
  };

  const renderLEDs = () => {
    switch (currentMode) {
      // case 'ALL_OFF': {
      //   return <div className={`${styles.animation}`} style={{animation: 'blink 1s infinite'}}></div>;
      // }
      // case 'BLINK_ALL':
      //   return <div className={`${styles.animation}`} style={{animation: 'blink 1s infinite'}}></div>;
      // case 'BLINK_EVEN':
      // case 'BLINK_ODD':
      //   return (
      //     <div>
      //       <div className={`${styles.animation}`} style={{animation: 'blink 1s infinite', animationDelay: '0.5s'}}></div>
      //       <div className={styles.animation}></div>
      //       <div className={`${styles.animation}`} style={{animation: 'blink 1s infinite', animationDelay: '0.5s'}}></div>
      //     </div>
      //   );
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
                 <div className={`${styles.led} ${styles.animationRedLED}`}>meow</div>
                 <div className={`${styles.led} ${styles.animationBlueLED}`}></div>
                 <div className={`${styles.led} ${styles.animationGreenLED}`}></div>
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
