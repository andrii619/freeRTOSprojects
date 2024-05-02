

export type LEDMode = 'ALL_OFF' | 'BLINK_ALL' | 'BLINK_EVEN' | 'BLINK_ODD' | 'ALTERNATE';


//{"id": 1, "name": "Andrii is a schmeckle","ledModes": ledModes,"currentLEDMode":ledMode},
export interface LEDState {
    id: number;
    name: string;
    ledModes: Array<LEDMode>[];
    currentLEDMode: string;
}



