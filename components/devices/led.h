#ifndef _LED_H_
#define _LED_H_

/**
 * @brief 手动关闭LED灯
 * @param none
 * @retval none
*/
extern void led_off(void);

/**
 * @brief 灯光增强
 * @param none
 * @retval none
*/
extern void led_enhance(void);

/**
 * @brief 灯光减弱
 * @param none
 * @retval none
*/
extern void led_weaken(void);

/**
 * @brief 灯光亮度存储
 * @param none
 * @retval none
*/
extern void led_save(void);

/**
 * @brief 灯光亮度恢复
 * @param none
 * @retval none
*/
extern void led_restore(void);

#endif /* _LED_H_ */
