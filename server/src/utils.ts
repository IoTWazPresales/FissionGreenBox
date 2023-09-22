import * as crypto from 'crypto';
//////// UTILS

export function generateDifficult(count: number) {
    var _sym = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
    var str = "";

    for (let i = 0; i < count; i++) {
        const tmp = _sym[Math.round(Math.random() * (_sym.length - 1))];
        str += "" + tmp;
    }

    return str;
}

export function generate(count: number) {
    var _sym = "abcdefghijklmnopqrstuvwxyz1234567890";
    var str = "";
    for (let i = 0; i < count; i++) {
        const tmp = _sym[Math.round(Math.random() * (_sym.length - 1))];
        str += "" + tmp;
    }
    return str;
}

/** Loops over nested js object and overrides its value with setval. 
 * This is useful when a packet comes into the system we can assign 
 * a timestamp for each property.
 */
export function recursiveSetValue<T>(
    inputOrig: unknown,
    setval: unknown,
) {
    if (inputOrig == undefined) return setval;
    if (inputOrig == null) return setval;

    var input = JSON.parse(JSON.stringify(inputOrig));
    Object.keys(input).map((val, index) => {
        if (
            (typeof input[val] == "object") && (Array.isArray(input[val]) == false)
        ) {
            input[val] = recursiveSetValue(input[val], setval);
        } else {
            input[val] = setval;
        }
    });

    return input;
}

// deno-lint-ignore no-explicit-any
export function isObject(item: any) {
    return (item && typeof item === "object" && !Array.isArray(item));
}

export function mergeDeep(
    // deno-lint-ignore no-explicit-any
    target: Record<string, unknown | any>,
    // deno-lint-ignore no-explicit-any
    source: Record<string, unknown | any>,
) {
    const isObject = (obj: unknown) => obj && typeof obj === "object";

    if (!isObject(target) || !isObject(source)) {
        return source;
    }

    if ((target instanceof Date) || (source instanceof Date)) {
        return source;
    }

    Object.keys(source).forEach((key) => {
        const targetValue = target[key];
        const sourceValue = source[key];

        if (Array.isArray(targetValue) && Array.isArray(sourceValue)) {
            target[key] = targetValue.concat(sourceValue);
        } else if (isObject(targetValue) && isObject(sourceValue)) {
            target[key] = mergeDeep(Object.assign({}, targetValue), sourceValue);
        } else {
            target[key] = sourceValue;
        }
    });

    return target;
}

export function clone(obj: unknown): unknown {
    return JSON.parse(JSON.stringify(obj));
}

export function isJson(x: string | Buffer) {
    try {
        if (typeof x === "string") {
            const a = JSON.parse(x);
            return a;
        } else {
            const a = JSON.parse(x.toString());
            return a;
        }


    } catch (err) {
        return false;
    }
    return false;
}


export function hash(text: string): Promise<string> {
    return new Promise((resolve, reject) => {
        crypto.scrypt(text, 'sajdi9034dfjsdifodsmfkld', 64, (err, derivedKey) => {
            if (err) { reject(err); }
            return resolve(derivedKey.toString('hex'));
        });
    });
}

export async function verify(text: string, match: string) {
    let a = (await hash(text)).toString();
    return (a === match);
}

export function btoa(data: string) {
    return Buffer.from(data).toString('base64');
}

export function atob(b64Encoded: string) {
    return Buffer.from(b64Encoded, 'base64').toString()
}


export function flatten(inObj: any) {
    const res: any = {};

    (function recurse(obj, current?: any) {
        for (const key in obj) {
            if (key) {
                const value = obj[key];
                const newKey = current ? current + '.' + key : key; // joined key with dot
                if (value && typeof value === 'object') {
                    recurse(value, newKey); // it's a nested object, so do it again
                } else {
                    res[newKey] = value; // it's not an object, so set the property
                }
            }
        }
    })(inObj);

    return res;
}


export function objectByString(obj: any, str: string): any {
    str = str.replace(/\[(\w+)\]/g, ".$1"); // convert indexes to properties
    str = str.replace(/^\./, ""); // strip a leading dot
    const a = str.split(".");
    for (let i = 0, n = a.length; i < n; ++i) {
        const k = a[i];
        if (k in obj) {
            obj = obj[k];
        } else {
            return;
        }
    }
    return obj;
}