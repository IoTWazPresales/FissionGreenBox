
/** When you want to log something 
 * 
 * 
 * @param message - The first input number
 * @param data - The second input number
 * @returns The arithmetic mean of `x` and `y`
 *
 * @beta
*/
export interface LogEvent {
  message: string;
  data?: any;
  level: "error" | "warn" | "info" | "verbose" | "debug" | "silly";
  group?: string;
}

export class Logger {
  debug: boolean = true;

  constructor(parent?:any) {
    if (parent) console.log(parent.constructor.name)
  }

  log(message?: any, ...optionalParams: any[]) {
    if (this.debug === false) return;
      let name = (this.constructor && this.constructor.name) ? this.constructor.name : ''
      console.log(`${new Date().toISOString()} ${padString(name)}`,message, ...optionalParams);
  }

}


function padString(instr: any) {
  return String(instr + "                 ").slice(0, 15);
}

export const logger = new Logger();